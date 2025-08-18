// 
// VOLT
// Copyright © 2025 starfrost
//
// cpu8086_ops_group.cpp: Admittedly this is kind of arbitrary
//

#include <emulation/emulation.hpp>
#include <emulation/components/cpu/cpu8086.hpp>

namespace Volt
{

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

        // IMPORTANT INFORMATION: This code FUCKING SUCKS. FIX IT!!!
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
                        if (value16 & 0x8000)
                            flags |= CPU8086Flags::Carry;

                        // use old value to determine overflow for left rotate (convert to bool?)
                        if (value16 & 0x8000 != value16 & 0x4000)
                            flags |= (flags & CPU8086Flags::Overflow);

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
                        if (value8 & 0x80)
                            flags |= CPU8086Flags::Carry;

                        //use old value to determine overflow for left rotate (convert to bool?)
                        if (value8 & 0x80 != value8 & 0x40)
                            flags |= (flags & CPU8086Flags::Overflow);

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
                        if (value16 & 0x01)
                            flags |= CPU8086Flags::Carry;

                        //update value
                        value16 = *modrm.ea_ptr;

                        //use new value to determine overflow for right rotate  (convert to bool?)
                        if ((value16 & 0x8000) != (value16 & 0x4000))
                            flags |= (flags & CPU8086Flags::Overflow);
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
                        if (value8 & 0x01)
                            flags |= CPU8086Flags::Carry;

                        value8 = *(uint8_t*)modrm.ea_ptr;

                        //use new value to determine overflow for right rotate (convert to bool?)
                        if ((value8 & 0x80) != (value8 & 0x40))
                            flags |= (flags & CPU8086Flags::Overflow);
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
                        if (value16 & 0x01)
                            flags |= CPU8086Flags::Carry;

                        //use old value to determine overflow for left shift (convert to bool?)
                        if ((value16 & 0x8000) != (value16 & 0x4000))
                            flags |= (flags & CPU8086Flags::Overflow);

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
                        if (value8 & 0x01)
                            flags |= CPU8086Flags::Carry;

                        //use old value to determine overflow for left shift (convert to bool?)
                        if ((value8 & 0x80) != (value8 & 0x40))
                            flags |= (flags & CPU8086Flags::Overflow);

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
                        if (value16 & 0x01)
                            flags |= CPU8086Flags::Carry;

                        //update value
                        value16 = *modrm.ea_ptr;

                        //use new value to determine overflow for right shift (convert to bool?)
                        if ((value16 & 0x8000) != (value16 & 0x4000))
                            flags |= CPU8086Flags::Overflow;

                        SetPZSFlags16(value16);
                        
                        if (value16 & 0x10)   
                            flags |= CPU8086Flags::AuxCarry;
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
                        if (value8 & 0x01)
                            flags |= CPU8086Flags::Carry;

                        value8 = *(uint8_t*)modrm.ea_ptr;

                        //use new value to determine overflow for right rotate (convert to bool?)
                        if ((value8 & 0x80) != (value8 & 0x40))
                            flags |= CPU8086Flags::Overflow;

                        SetPZSFlags8(value8);
                        
                        flags &= ~CPU8086Flags::AuxCarry;
                    }
                }
                break;
        }
    }
}