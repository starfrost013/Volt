// 
// VOLT
// Copyright © 2025 starfrost
//
// cpu8086_modrm.cpp: Super Happy Fun Time ModR/M 
//
#include <emulation/components/cpu/cpu8086.hpp>

namespace Volt
{
    CPU8086::CPU8086InstructionModRM CPU8086::Decode_ModRM(uint8_t opcode)
    {
        CPU8086InstructionModRM modrm_decode = {0}; 

        bool w = opcode & 0x01;
        
        modrm_decode.modrm = Prefetch_Pop8();

        // handle reg part (ignored in certain cases)
        // length can be handled by the opcode function -- it's part of the opcode anyway
        modrm_decode.reg_ptr8 = register_table8[modrm_decode.reg];
        modrm_decode.reg_ptr16 = register_table16[modrm_decode.reg];

        // default = DS for this part.
        uint16_t reg = ds;
        uint32_t final_linear_address = 0;      //for mod=0/1/2

        switch (modrm_decode.mod)
        {
            default:
                // Disp16
                if (modrm_decode.mod == 0x00 
                    && modrm_decode.rm == 6)
                {
                        modrm_decode.ea_ptr = (uint16_t*)&address_space->access_byte[linear_pc() + 2];

                        Prefetch_Pop16(); // pop prefetch, but don't return a pointer to it
                        return modrm_decode; //return early
                }
                 // now decode the RM
                
                switch (seg_override)
                {
                    case seg_override_es:
                        reg = es;
                        break;
                    case seg_override_ss:
                        reg = ss;
                        break; 
                    case seg_override_cs:
                        reg = cs;
                        break;
                }

                final_linear_address = (reg << 4) + rm_table[modrm_decode.rm];

                //TODO: PUT ADDRESS SPACE SIZE IN ADDRESS SPACE SYSTEM
                // Prevent us going to invalid memory in the case of the CPU going wild
                final_linear_address %= CPU8086_ADDR_SPACE_SIZE;

                modrm_decode.ea_ptr = (uint16_t*)&address_space->access_byte[final_linear_address];

                if (modrm_decode.mod == 0x01) // +disp8
                {
                    // make pointer arithmetic a bit easier here
                    uint8_t* temp = (uint8_t*)modrm_decode.ea_ptr;
                    temp += Prefetch_Pop8();
                    modrm_decode.ea_ptr = (uint16_t*)temp;
                    return modrm_decode;
                }
                else if (modrm_decode.mod == 0x02) // +disp16
                {
                    uint8_t* temp = (uint8_t*)modrm_decode.ea_ptr;
                    temp += Prefetch_Pop16();
                    modrm_decode.ea_ptr = (uint16_t*)temp;
 
                    return modrm_decode;
                }

                break;        
            case 0x03:  // 11
                // This is a bit stupid. We cast it to 8 bit for 8 bit opcodes.
                if (!w)
                    modrm_decode.ea_ptr = (uint16_t*)register_table8[modrm_decode.rm];
                else
                    modrm_decode.ea_ptr = register_table16[modrm_decode.rm];
                
                return modrm_decode;         
        }

        Logging_LogChannel("808x: Invalid ModR/M decode mod-reg-rm %d-%d-%d", LogChannel::Warning, modrm_decode.mod, modrm_decode.reg, modrm_decode.rm);
        return modrm_decode;
    }

}