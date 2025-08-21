// 
// VOLT
// Copyright © 2025 starfrost
//
// cpu8086_ops_control.cpp: 8086 control flow ops
//

#include "cpu8086.hpp"
#include <emulation/emulation.hpp>
#include <emulation/components/cpu/cpu8086.hpp>

namespace Volt
{
    void CPU8086::Op_JmpFar(uint8_t opcode)
    {
        uint16_t new_ip = Prefetch_Pop16();
        uint16_t new_cs = Prefetch_Pop16();

        cs = new_cs;
        ip = new_ip;

        // flush the prefetch queue
        Prefetch_Flush(); 
    }

    void CPU8086::Op_CallFar(uint8_t opcode)
    {
        uint16_t new_ip = Prefetch_Pop16();
        uint16_t new_cs = Prefetch_Pop16();

        stack_push_16(cs); 
        stack_push_16(ip); 

        cs = new_cs;
        ip = new_ip;

        // flush the prefetch queue
        Prefetch_Flush(); 
    }
    
    void CPU8086::Op_ShortJmp(uint8_t opcode)
    {
        // this is inverted from the usual "is 16 bit" check
        bool w = !(opcode & 0x02);

        int16_t offset16 = 0;
        int8_t offset8 = 0;

        if (w)
        {
            offset16 = (int16_t)Prefetch_Pop16(); 
            ip += offset16;
        }
        else
        {
            offset8 = (int8_t)Prefetch_Pop8(); 
            ip += offset8;
        }

        //to be safe. i assume real cpu does this
        Prefetch_Flush(); 
    }

    void CPU8086::Op_ShortConditionalJmp(uint8_t opcode)
    {
        uint8_t condition = (opcode & 0x0F);

        bool take_branch;

        // COULD reduce this to 8 condtiions...
        switch (condition)
        {
            case 0x00:  
                take_branch = (flags & CPU8086Flags::Overflow);
                break;
            case 0x01:
                take_branch = !(flags & CPU8086Flags::Overflow);
                break;
            case 0x02: 
                take_branch = (flags & CPU8086Flags::Carry);
                break;
            case 0x03: 
                take_branch = !(flags & CPU8086Flags::Carry);
                break;
            case 0x04: 
                take_branch = (flags & CPU8086Flags::Zero);
                break;
            case 0x05: 
                take_branch = !(flags & CPU8086Flags::Zero);
                break;
            case 0x06:
                take_branch = ((flags & CPU8086Flags::Carry) | (flags & CPU8086Flags::Zero));
                break;
            case 0x07:
                take_branch = !((flags & CPU8086Flags::Carry) | (flags & CPU8086Flags::Zero));
                break;              
            case 0x08:  
                take_branch = (flags & CPU8086Flags::Sign);
                break;
            case 0x09:
                take_branch = !(flags & CPU8086Flags::Sign);
                break;
            case 0x0A:  
                take_branch = (flags & CPU8086Flags::Parity);
                break;
            case 0x0B:
                take_branch = !(flags & CPU8086Flags::Parity);
                break;
            case 0x0C:
                take_branch = ((flags & CPU8086Flags::Sign) ^ (flags & CPU8086Flags::Overflow));
                break;
            case 0x0D:
                take_branch = !((flags & CPU8086Flags::Sign) ^ (flags & CPU8086Flags::Overflow));
                break;            
            case 0x0E:
                take_branch = (((flags & CPU8086Flags::Sign) ^ (flags & CPU8086Flags::Overflow)) | (flags & CPU8086Flags::Zero));
                break;
            case 0x0F:
                take_branch = !(((flags & CPU8086Flags::Sign) ^ (flags & CPU8086Flags::Overflow)) | (flags & CPU8086Flags::Zero));
                break;
        }

        int8_t amount = (int8_t)Prefetch_Pop8();

        if (take_branch)
        {
            ip += amount; //+128->-127
            Prefetch_Flush();
        }
    }

    void CPU8086::Op_RetNear(uint8_t opcode)
    {
        bool use_imm = !(opcode & 0x01);
        ip = stack_pop_16();

        if (use_imm)
            sp += Prefetch_Pop8(); 

        Prefetch_Flush();
    }

    void CPU8086::Op_RetFar(uint8_t opcode)
    {
        bool use_imm = !(opcode & 0x01);
        ip = stack_pop_16();
        cs = stack_pop_16();

        if (use_imm)
            sp += Prefetch_Pop8(); 

        Prefetch_Flush();
    }

    // Halt
    void CPU8086::Op_Hlt(uint8_t opcode)
    {
        // Halt the CPU.
        halted = true;

        Logging_LogChannel("The CPU is HALTED. Send NMI or, if IF=1, an interrupt to wake it up.", LogChannel::Message);
    }

    // LOOP assembly optimisation instruction
    void CPU8086::Op_Loop(uint8_t opcode)
    {
        uint8_t op = opcode & 0x03;
        int8_t offset = (int8_t)Prefetch_Pop8();

        if (op == 0x03
        && !cx) // JCXZ
        {   
            ip += offset;
            Prefetch_Flush();
            return;
        }

        // decrement
        cx--; 
        
        bool keep_going = false;
    
        switch (op)
        {
            case 0x00:          // LOOPNZ
                keep_going = (cx != 0) && !(flags & CPU8086Flags::Zero);
                break;
            case 0x01:          // LOOPE/LOOPZ
                keep_going = (cx != 0) && (flags & CPU8086Flags::Zero);
                break;
            case 0x02:          // LOOP
                keep_going = (cx != 0);
                break;
        }

        disasm_suppress_logging = (keep_going);

        if (keep_going)
        {
            ip += offset;
            Prefetch_Flush();
        }
    }
}