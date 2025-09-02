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

        if (!reverse)
            *modrm.ea_ptr = *segreg_ptr;
        else
            *segreg_ptr = *modrm.ea_ptr;
    }

    // A0-A3 move
    void CPU8086::Op_MovOffset(uint8_t opcode)
    {
        //use the segment override when necessary
        uint16_t seg_base = (seg_override == seg_override_none) ? ds : *seg_override_reg_ptr;

        uint32_t seg_offset = (((uint32_t)seg_base << 4) + Prefetch_Pop16()) % address_space->size;

        bool w = (opcode & 0x01);
        bool dir = (opcode & 0x02);

        if (!dir)
            (w) ? ax = address_space->read_word(seg_offset) : al = address_space->read_byte(seg_offset);
        else // todo: make byte like (d)word
        {
            if (w) 
                address_space->write_word(seg_offset, ax);
            else
                address_space->write_byte(seg_offset, al);
        }
    }

    // 9x XCHG
    void CPU8086::Op_XchgReg(uint8_t opcode)
    {
        uint16_t old_ax = ax;
        uint16_t reg = (*register_table16[opcode & 0x07]);
        ax = reg;
        reg = old_ax;
    }

    void CPU8086::Op_XchgModRM(uint8_t opcode)
    {
        bool w = (opcode & 0x01);
        CPU8086InstructionModRM modrm = Decode_ModRM(opcode);

        if (w)
        {
            uint16_t old_reg16 = *modrm.reg_ptr16;
            *modrm.reg_ptr16 = *modrm.ea_ptr;
            *modrm.ea_ptr = old_reg16;
        }
        else
        {
            uint8_t old_reg8 = *modrm.reg_ptr8;
            *modrm.reg_ptr8 = *(uint8_t*)modrm.ea_ptr;
            *(uint8_t*)modrm.ea_ptr = old_reg8;
        }
    }

    void CPU8086::Op_LesLds(uint8_t opcode)
    {
        bool is_lds = (opcode & 0x01);

        CPU8086InstructionModRM modrm = Decode_ModRM(opcode);

        uint16_t* reg = register_table16[modrm.reg];

        //use the segment override when necessary
        uint16_t seg_base = (seg_override == seg_override_none) ? ds : *seg_override_reg_ptr;
        uint32_t final_addr = (((uint32_t)seg_base << 4) + rm_table[modrm.rm]) % address_space->size;

        // yes we have to add the displacement again too
        final_addr += modrm.displacement;

        *reg = address_space->read_word(final_addr);

        if (is_lds)
            ds = address_space->read_word((final_addr + 2) & (address_space_primary->size - 1));
        else
            es = address_space->read_word((final_addr + 2) & (address_space_primary->size - 1));
    }


    void CPU8086::Op_Lea(uint8_t opcode)
    {
        CPU8086InstructionModRM modrm = Decode_ModRM(opcode);

        // Since *ea_ptr is a pointer and the segment gets effectively ignored we have to spcial case this and basically do the calculation "again"
        uint16_t seg_base = (seg_override == seg_override_none) ? ds : *seg_override_reg_ptr;
        uint32_t final_addr = (((uint32_t)seg_base << 4) + rm_table[modrm.rm]) % address_space->size;

        // yes we have to add the displacement again too
        final_addr += modrm.displacement;

        *register_table16[modrm.reg] = (uint16_t)(final_addr & 0xFFFF);//only lower 16 bits get stored

    }

}