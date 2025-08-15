// 
// VOLT
// Copyright © 2025 starfrost
//
// cpu8086_modrm.cpp: Super Happy Fun Time ModR/M 
//
#include <emulation/components/cpu/cpu8086.hpp>

namespace Volt
{
    CPU8086::CPU8086InstructionModRM CPU8086::Decode_ModRM(bool w, uint8_t modrm)
    {
        CPU8086InstructionModRM modrm_decode = {0}; 

        modrm_decode.modrm = address_space->access_byte[linear_pc + 1];

        // handle reg part (ignored in certain cases)
        // length can be handled by the opcode function -- it's part of the opcode anyway
        modrm_decode.reg_ptr8 = register_table8[modrm_decode.reg];
        modrm_decode.reg_ptr16 = register_table16[modrm_decode.reg];

        switch (modrm_decode.mod)
        {
            default:
                // Disp16
                if (modrm_decode.mod == 0x00 
                    && modrm_decode.rm == 6)
                    {
                        modrm_decode.ea_ptr = &address_space->access_word[linear_pc + 2];
                        return modrm_decode; //return early
                    }

                // now decode the RM
                modrm_decode.ea_ptr = rm_table[modrm_decode.rm];

                if (modrm_decode.mod == 0x01) // +disp8
                {
                    // make pointer arithmetic a bit easier here
                    uint8_t* temp = (uint8_t*)modrm_decode.ea_ptr;
                    temp += (address_space->access_byte[linear_pc + 2]);
                    modrm_decode.ea_ptr = (uint16_t*)temp;
                    return modrm_decode;
                }
                else if (modrm_decode.mod == 0x02) // +disp16
                {
                    uint8_t* temp = (uint8_t*)modrm_decode.ea_ptr;
                    temp += (address_space->access_word[linear_pc + 2]);
                    modrm_decode.ea_ptr = (uint16_t*)temp;
 
                    return modrm_decode;
                }
            
            case 0x03:  // 11
                // This is a bit stupid. We cast it to 8 bit for 8 bit opcodes.
                if (!w)
                    modrm_decode.ea_ptr = (uint16_t*)register_table8[modrm_decode.rm];
                else
                    modrm_decode.ea_ptr = register_table16[modrm_decode.rm];
                
                return modrm_decode;         
        }
    }
}