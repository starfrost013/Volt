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

        //Logging_LogChannel("JMPF %04x:%04x", LogChannel::Debug, new_cs, new_ip);

        // flush the prefetch queue
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

        if (take_branch)
        {
            ip += (int8_t)Prefetch_Pop8(); //+128->-127
            Prefetch_Flush();
        }
    }
}