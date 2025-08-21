// 
// VOLT
// Copyright © 2025 starfrost
//
// cpu8086_ops_move.cpp: 8086 move ops
//

#include <emulation/emulation.hpp>
#include <emulation/components/cpu/cpu8086.hpp>

namespace Volt
{
    void CPU8086::Op_MovImmedToReg(uint8_t opcode)
    {
        // This works because we only care about 40-4f
        if (opcode & 0x08)
            *register_table16[opcode % 0x08] = Prefetch_Pop16();
        else
            *register_table8[opcode % 0x08] = Prefetch_Pop8();
    }

    // C6 / C7
    void CPU8086::Op_MovImmedToModRM(uint8_t opcode)
    {
        CPU8086InstructionModRM modrm = Decode_ModRM(opcode);

        bool w = (opcode & 0x01);

        uint16_t immed16 = 0x00;
        uint8_t immed8 = 0;

        if (w)
        {
            immed16 = Prefetch_Pop16();
            *modrm.ea_ptr = immed16;
        } 
        else
        {
            immed8 = Prefetch_Pop8();
            *(uint8_t*)modrm.ea_ptr = immed8;
        }
    }

    void CPU8086::Op_MovModRM(uint8_t opcode)
    {
        CPU8086InstructionModRM modrm = Decode_ModRM(opcode);

        bool w = (opcode & 0x01);
        bool reverse = (opcode & 0x02);

        if (reverse)
            (w) ? *modrm.reg_ptr16 = *modrm.ea_ptr : *modrm.reg_ptr8 = *(uint8_t*)modrm.ea_ptr;
        else
            (w) ? *modrm.ea_ptr = *modrm.reg_ptr16 : *(uint8_t*)modrm.ea_ptr = *modrm.reg_ptr8;
    }

    void CPU8086::Op_MovRegToSeg(uint8_t opcode)
    {
        CPU8086InstructionModRM modrm = Decode_ModRM(opcode);

        // this one is only 16bit. 8d is LEA
        bool reverse = (opcode & 0x02);

        uint16_t* segreg_ptr = segreg_table[modrm.reg];

        if (!segreg_ptr)
        {
            Logging_LogChannel("It's all gone very wrong. Invalid segreg %s in MOV!", LogChannel::Warning, segreg_table_disasm[modrm.reg]);
            return;
        }

        if (reverse)
            *modrm.ea_ptr = *segreg_ptr;
        else
            *segreg_ptr = *modrm.ea_ptr;
    }
}