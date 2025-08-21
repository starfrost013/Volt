// 
// VOLT
// Copyright © 2025 starfrost
//
// cpu8086_ops_group.cpp: Admittedly this is kind of arbitrary
//

#include "cpu8086.hpp"
#include <emulation/emulation.hpp>
#include <emulation/components/cpu/cpu8086.hpp>

namespace Volt
{
    #define CPU8086_GRP1_OP_ADD     0x00
    #define CPU8086_GRP1_OP_OR      0x01
    #define CPU8086_GRP1_OP_ADC     0x02
    #define CPU8086_GRP1_OP_SBB     0x03
    #define CPU8086_GRP1_OP_AND     0x04
    #define CPU8086_GRP1_OP_SUB     0x05
    #define CPU8086_GRP1_OP_XOR     0x06
    #define CPU8086_GRP1_OP_CMP     0x07

    #define CPU8086_GRP2_OP_ROL     0x00
    #define CPU8086_GRP2_OP_ROR     0x01
    #define CPU8086_GRP2_OP_RCL     0x02
    #define CPU8086_GRP2_OP_RCR     0x03
    #define CPU8086_GRP2_OP_SHL     0x04
    #define CPU8086_GRP2_OP_SHR     0x05
    #define CPU8086_GRP2_OP_SETMO   0x06
    #define CPU8086_GRP2_OP_SAR     0x07

    #define CPU8086_GRP2_IS_16BIT   0x01
    #define CPU8086_GRP2_USE_CL     0x02

    #define CPU8086_GRP45_OP_INC    0x00
    #define CPU8086_GRP45_OP_DEC    0x01
    #define CPU8086_GRP4_MAX_VALID  0x01
    #define CPU8086_GRP45_OP_CALL   0x02
    #define CPU8086_GRP45_OP_CALLF  0x03            // Mem only
    #define CPU8086_GRP45_OP_JMP    0x04
    #define CPU8086_GRP45_OP_JMPF   0x05
    #define CPU8086_GRP45_OP_PUSH   0x06
    #define CPU8086_GRP45_OP_PUSHI  0x07            // Illegal PUSH

    void CPU8086::Op_Grp1(uint8_t opcode)
    {
        CPU8086InstructionModRM modrm = Decode_ModRM(opcode);

        uint8_t immed8 = 0x00;
        uint16_t immed16 = 0x00;

        bool is_8on8 = ((opcode & 0x03) == 0x00 || (opcode & 0x03) == 0x02);
        bool is_8on16 = ((opcode & 0x03) == 0x03);
        bool is_16on16 = ((opcode & 0x03) == 0x01);

        if (is_8on8 || is_16on16)
            immed8 = Prefetch_Pop8();
        else
            immed16 = Prefetch_Pop16();

        // switch function based on opcode + modrm.reg 
        switch (modrm.reg)
        {
            case CPU8086_GRP1_OP_ADD:
                if (is_8on8)
                    Op_AddInternal8to8((uint8_t*)modrm.ea_ptr, &immed8);
                else if (is_8on16)
                    Op_AddInternal8to16(modrm.ea_ptr, &immed8);
                else if (is_16on16)
                    Op_AddInternal16to16(modrm.ea_ptr, &immed16);
                break;
            case CPU8086_GRP1_OP_OR:
                if (is_8on8)
                    Op_OrInternal8to8((uint8_t*)modrm.ea_ptr, &immed8);
                else if (is_8on16)
                    Op_OrInternal8to16(modrm.ea_ptr, &immed8);
                else if (is_16on16)
                    Op_OrInternal16to16(modrm.ea_ptr, &immed16);
                break;
            case CPU8086_GRP1_OP_ADC:
                if (is_8on8)
                    Op_AdcInternal8to8((uint8_t*)modrm.ea_ptr, &immed8);
                else if (is_8on16)
                    Op_AdcInternal8to16(modrm.ea_ptr, &immed8);
                else if (is_16on16)
                    Op_AdcInternal16to16(modrm.ea_ptr, &immed16);
                break;
            case CPU8086_GRP1_OP_SBB:
                if (is_8on8)
                    Op_SbbInternal8to8((uint8_t*)modrm.ea_ptr, &immed8);
                else if (is_8on16)
                    Op_SbbInternal8to16(modrm.ea_ptr, &immed8);
                else if (is_16on16)
                    Op_SbbInternal16to16(modrm.ea_ptr, &immed16);
                break;
            case CPU8086_GRP1_OP_AND:
                if (is_8on8)
                    Op_AndInternal8to8((uint8_t*)modrm.ea_ptr, &immed8);
                else if (is_8on16)
                    Op_AndInternal8to16(modrm.ea_ptr, &immed8);
                else if (is_16on16)
                    Op_AndInternal16to16(modrm.ea_ptr, &immed16);
                break;
            case CPU8086_GRP1_OP_SUB:
                if (is_8on8)
                    Op_SubInternal8to8((uint8_t*)modrm.ea_ptr, &immed8);
                else if (is_8on16)
                    Op_SubInternal8to16(modrm.ea_ptr, &immed8);
                else if (is_16on16)
                    Op_SubInternal16to16(modrm.ea_ptr, &immed16);
                break;
            case CPU8086_GRP1_OP_XOR:
                if (is_8on8)
                    Op_XorInternal8to8((uint8_t*)modrm.ea_ptr, &immed8);
                else if (is_8on16)
                    Op_XorInternal8to16(modrm.ea_ptr, &immed8);
                else if (is_16on16)
                    Op_XorInternal16to16(modrm.ea_ptr, &immed16);
                break;
            case CPU8086_GRP1_OP_CMP:
                if (is_8on8)
                    Op_CmpInternal8to8((uint8_t*)modrm.ea_ptr, &immed8);
                else if (is_8on16)
                    Op_CmpInternal8to16(modrm.ea_ptr, &immed8);
                else if (is_16on16)
                    Op_CmpInternal16to16(modrm.ea_ptr, &immed16);
                break;
        }
    }

    // ROL, ROR, RCL, RCR, SHL, SHR, SETMO/SETMOC, SAR
    //TODO: REALISTIC TIMINGS
    void CPU8086::Op_Grp2(uint8_t opcode)
    {
        CPU8086InstructionModRM modrm = Decode_ModRM(opcode);

        uint8_t num_bits = 1;
        
        if (opcode & CPU8086_GRP2_USE_CL) // use CL
            num_bits = cl;

        uint16_t value16 = *modrm.ea_ptr;
        uint8_t value8 = *(uint8_t*)modrm.ea_ptr;

        bool is_16bit = (is_16bit);

        // TODO: IMPORTANT INFORMATION: This code FUCKING SUCKS. FIX IT!!!
        switch (modrm.reg)
        {
            // ROL / RCL (Rotate Left) - DO NOT SHIFT
            case CPU8086_GRP2_OP_ROL:
            case CPU8086_GRP2_OP_RCL:
                if (is_16bit)  //16bit
                {
                    for (uint32_t i = 0; i < num_bits; i++)
                    {
                        *modrm.ea_ptr <<= 1;

                        if (modrm.reg != 0x02)                      // RCL
                            *modrm.ea_ptr |= (value16 >> 15);
                        else
                            *modrm.ea_ptr |= (flags & CPU8086Flags::Carry) >> 15;

                        //use old  value to determine carry
                        (value16 & 0x8000) ? flags |= CPU8086Flags::Carry : flags &= ~CPU8086Flags::Carry;

                        // use old value to determine overflow for left rotate (convert to bool?)
                        ((value16 & 0x8000) != (value16 & 0x4000)) ? flags |= CPU8086Flags::Overflow : flags &= ~CPU8086Flags::Overflow;


                        //update value
                        value16 = *modrm.ea_ptr;
                    }
                }
                else
                {
                    for (uint32_t i = 0; i < num_bits; i++)
                    {
                        *(uint8_t*)modrm.ea_ptr <<= 1;

                        if (modrm.reg != 0x02)                      // RCL
                            *(uint8_t*)modrm.ea_ptr |= (value8 >> 7);
                        else
                            *(uint8_t*)modrm.ea_ptr |= (flags & CPU8086Flags::Carry) >> 7;

                        //use old value to determine carry
                        (value8 & 0x80) ? flags |= CPU8086Flags::Carry : flags &= ~CPU8086Flags::Carry;

                        //use old value to determine overflow for left rotate (convert to bool?)
                        ((value8 & 0x80) != (value8 & 0x40)) ? flags |= CPU8086Flags::Overflow : flags &= ~CPU8086Flags::Overflow;

                        //update value
                        value8 = *(uint8_t*)modrm.ea_ptr;
                    }
                }
                break;
            // ROR / RCR (Rotate Right) - DO NOT SHIFT
            case CPU8086_GRP2_OP_ROR:
            case CPU8086_GRP2_OP_RCR:
                if (is_16bit)  //16bit
                {
                    for (uint32_t i = 0; i < num_bits; i++)
                    {
                        *modrm.ea_ptr >>= 1;

                        if (modrm.reg != 0x02)                      // RCR
                            *modrm.ea_ptr |= (value16 << 15);
                        else
                            *modrm.ea_ptr |= (flags & CPU8086Flags::Carry) << 15;

                        //use old value to determine carry
                        (value16 & 0x01) ? flags |= CPU8086Flags::Carry : flags &= ~CPU8086Flags::Carry;

                        //update value
                        value16 = *modrm.ea_ptr;

                        //use new value to determine overflow for right rotate  (convert to bool?)
                        ((value16 & 0x8000) != (value16 & 0x4000)) ? flags |= CPU8086Flags::Overflow : flags &= ~CPU8086Flags::Overflow;

                    }
                }
                else
                {
                    for (uint32_t i = 0; i < num_bits; i++)
                    {
                        *(uint8_t*)modrm.ea_ptr >>= 1;

                        if (modrm.reg != 0x02)                     
                            *(uint8_t*)modrm.ea_ptr |= (value8 << 7);
                        else
                            *(uint8_t*)modrm.ea_ptr |= (flags & CPU8086Flags::Carry) << 7;

                        //update value

                        //use old value to determine carry
                        (value8 & 0x01) ? flags |= CPU8086Flags::Carry : flags &= ~CPU8086Flags::Carry;

                        value8 = *(uint8_t*)modrm.ea_ptr;

                        //use new value to determine overflow for right rotate (convert to bool?)
                        ((value8 & 0x80) != (value8 & 0x40)) ? flags |= CPU8086Flags::Overflow : flags &= ~CPU8086Flags::Overflow;
                    }
                }
                break;
            // SHL/SAL (Shift Left) 
            case CPU8086_GRP2_OP_SHL:
                if (is_16bit)  //16bit
                {
                    for (uint32_t i = 0; i < num_bits; i++)
                    {
                        *modrm.ea_ptr <<= 1;

                        //use old value to determine carry
                        (value16 & 0x80) ? flags |= CPU8086Flags::Carry : flags &= ~CPU8086Flags::Carry;

                        //use old value to determine overflow for left shift (convert to bool?)
                        ((value16 & 0x8000) != (value16 & 0x4000)) ? flags |= CPU8086Flags::Overflow : flags &= ~CPU8086Flags::Overflow;

                        //update value
                        value16 = *modrm.ea_ptr;

                        SetPZSFlags16(value16);
                        
                        if (value16 & 0x10)   
                            flags |= CPU8086Flags::AuxCarry;
                    }
                }
                else
                {
                    for (uint32_t i = 0; i < num_bits; i++)
                    {
                        *(uint8_t*)modrm.ea_ptr <<= 1;

                        if (modrm.reg != 0x02)                     
                            *(uint8_t*)modrm.ea_ptr |= (value8 << 7);
                        else
                            *(uint8_t*)modrm.ea_ptr |= (flags & CPU8086Flags::Carry) << 7;

                        //use old value to determine carry
                        (value8 & 0x01) ? flags |= CPU8086Flags::Carry : flags &= ~CPU8086Flags::Carry;

                        //use old value to determine overflow for left shift (convert to bool?)
                        ((value8 & 0x80) != (value8 & 0x40)) ? flags |= CPU8086Flags::Overflow : flags &= ~CPU8086Flags::Overflow;

                        value8 = *(uint8_t*)modrm.ea_ptr;

                        SetPZSFlags8(value8);
                        
                        if (value8 & 0x10)   
                            flags |= CPU8086Flags::AuxCarry;
                    }
                }
                break;
            // SETMO/SETMOC Undocumented operation
            // https://www.os2museum.com/wp/undocumented-8086-opcodes-part-i/
            case CPU8086_GRP2_OP_SETMO:
                if (opcode & CPU8086_GRP2_USE_CL) // Use CL
                {
                    if (is_16bit) //16bit
                        *modrm.ea_ptr = (cl != 0) ? 0xFFFF : 0x00;
                    else
                        *(uint8_t*)modrm.ea_ptr = (cl != 0) ? 0xFF : 0x00;
                }
                else
                {
                    if (is_16bit) //16bit
                        *modrm.ea_ptr = 0xFFFF;
                    else
                        *(uint8_t*)modrm.ea_ptr = 0xFF;
                }
                break;
            // SHR / SAR
            case CPU8086_GRP2_OP_SHR:
            case CPU8086_GRP2_OP_SAR: 
                if (is_16bit)  //16bit
                {
                    for (uint32_t i = 0; i < num_bits; i++)
                    {
                        if (modrm.reg == CPU8086_GRP2_OP_SHR)
                            *modrm.ea_ptr >>= 1;
                        else 
                            *modrm.ea_ptr >>= 1 | (value16 & 0x8000); 

                        //use old value to determine carry
                        (value16 & 0x01) ? flags |= CPU8086Flags::Carry : flags &= ~CPU8086Flags::Carry;

                        //update value
                        value16 = *modrm.ea_ptr;

                        //use new value to determine overflow for right shift (convert to bool?)
                        ((value16 & 0x8000) != (value16 & 0x4000)) ? flags |= CPU8086Flags::Overflow : flags &= ~CPU8086Flags::Overflow;

                        SetPZSFlags16(value16);
                    }
                }
                else
                {
                    for (uint32_t i = 0; i < num_bits; i++)
                    {
                        if (modrm.reg == CPU8086_GRP2_OP_SHR)
                            *(uint8_t*)modrm.ea_ptr >>= 1;
                        else 
                            *(uint8_t*)modrm.ea_ptr >>= 1 | (value8 & 0x80); 

                        //use old value to determine carry

                        // TODO: can this be done *after* op?
                        (value8 & 0x01) ? flags |= CPU8086Flags::Carry : flags &= ~CPU8086Flags::Carry;

                        value8 = *(uint8_t*)modrm.ea_ptr;

                        //use new value to determine overflow for right rotate (convert to bool?)
                        // TODO: can this be done *after* op?
                        ((value8 & 0x80) != (value8 & 0x40)) ? flags |= CPU8086Flags::Overflow : flags &= ~CPU8086Flags::Overflow;

                        SetPZSFlags8(value8);
                    }
                }
                
                flags &= ~CPU8086Flags::AuxCarry;
                break;
        }
    }


    void CPU8086::Op_Grp45(uint8_t opcode)
    {
        CPU8086InstructionModRM modrm = Decode_ModRM(opcode);

        // FE = grp4
        // FF = grp5
        bool w = (opcode & 0x01);

        if (!w && modrm.reg > CPU8086_GRP4_MAX_VALID)
            Logging_LogChannel("What you're about to execute isn't going to end very well", LogChannel::Warning);

        switch (modrm.reg)
        {
            //INC DEC are pretty trivial so may as well duplicate them here
            case CPU8086_GRP45_OP_INC:
                if (w)
                {
                    uint16_t result = ++(*modrm.ea_ptr);
                    SetPZSFlags16(result);
                      
                    if (result & 0x0F == 0x00) 
                        flags |= CPU8086Flags::AuxCarry;

                    SetOF16_Add(result, result - 1, 1);
                }
                else
                {
                    uint8_t result = ++(*(uint8_t*)modrm.ea_ptr);
                    SetPZSFlags8(result);

                    if (result & 0x0F == 0x00) 
                        flags |= CPU8086Flags::AuxCarry;

                    SetOF8_Add(result, result - 1, 1);
                }

                break;
            case CPU8086_GRP45_OP_DEC:
                if (w)
                {
                    uint16_t result = --(*modrm.ea_ptr);
                    SetPZSFlags16(result);
                      
                    if (result & 0x0F == 0x0F) 
                        flags |= CPU8086Flags::AuxCarry;

                    SetOF16_Sub(result, result + 1, 1);
                }
                else
                {
                    uint8_t result = --(*(uint8_t*)modrm.ea_ptr);
                    SetPZSFlags8(result);

                    if (result & 0x0F == 0x0F) 
                        flags |= CPU8086Flags::AuxCarry;

                    SetOF8_Sub(result, result + 1, 1);
                }
                break;
            case CPU8086_GRP45_OP_CALL:
                if (w)
                {
                    uint16_t ip_new = *modrm.ea_ptr; 
                    stack_push_16(ip);
                    ip = ip_new; 
                }
                else
                {
                    uint8_t ip_new_messed = *(uint8_t*)modrm.ea_ptr;
                    stack_push_16(ip);
                    ip = (ip >> 8) << 8 | ip_new_messed;
                }

                Prefetch_Flush();
                break;
            case CPU8086_GRP45_OP_CALLF:
                if (w)
                {
                    uint16_t ip_new = *modrm.ea_ptr; 
                    uint16_t cs_new = *(modrm.ea_ptr + 1) % address_space->size;       //1 due to uint16_t
                    stack_push_16(cs);
                    stack_push_16(ip);

                    cs = cs_new; 
                    ip = ip_new; 
                }
                else
                {
                    uint8_t ip_new_messed = *(uint8_t*)modrm.ea_ptr;
                    uint8_t cs_new_messed = *(uint8_t*)(modrm.ea_ptr + 2) % address_space->size;
                    stack_push_16(cs); 
                    stack_push_16(ip);

                    ip = (ip >> 8) << 8 | ip_new_messed;
                    cs = (cs >> 8) << 8 | cs_new_messed;
                }

                Prefetch_Flush();
                break;
            case CPU8086_GRP45_OP_JMP:
                if (w)
                {
                    uint16_t ip_new = *modrm.ea_ptr; 
                    ip = ip_new; 
                }
                else
                {
                    uint8_t ip_new_messed = *(uint8_t*)modrm.ea_ptr;
                    ip = (ip >> 8) << 8 | ip_new_messed;
                }

                Prefetch_Flush();
                break;
            case CPU8086_GRP45_OP_JMPF:
                if (w)
                {
                    uint16_t ip_new = *modrm.ea_ptr; 
                    uint16_t cs_new = *(modrm.ea_ptr + 1) % address_space->size;       //1 due to uint16_t

                    cs = cs_new; 
                    ip = ip_new; 
                }
                else
                {
                    uint8_t ip_new_messed = *(uint8_t*)modrm.ea_ptr;
                    uint8_t cs_new_messed = *(uint8_t*)(modrm.ea_ptr + 2) % address_space->size;

                    ip = (ip >> 8) << 8 | ip_new_messed;
                    cs = (cs >> 8) << 8 | cs_new_messed;
                }

                Prefetch_Flush();
                break;
            case CPU8086_GRP45_OP_PUSH:
            case CPU8086_GRP45_OP_PUSHI:   
                if (w)
                {
                    // we have to do this for SP
                    if (modrm.ea_ptr == &sp)
                        stack_push_16(*(modrm.ea_ptr - 1));                                 // 1 due to uint16_t
                    else
                        stack_push_16(*modrm.ea_ptr);
                }
                else
                {
                    if (modrm.ea_ptr == &sp)
                        stack_push_16(*((uint8_t*)modrm.ea_ptr - 2));                        // 1 due to uint16_t
                    else
                        stack_push_16(*(uint8_t*)modrm.ea_ptr);
                }

                break;
        }
    }
}