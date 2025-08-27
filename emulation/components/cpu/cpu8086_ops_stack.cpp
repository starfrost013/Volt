// 
// VOLT
// Copyright © 2025 starfrost
//
// cpu8086_ops_move.cpp: 8086 move ops
//

#include "cpu8086.hpp"
#include <emulation/emulation.hpp>
#include <emulation/components/cpu/cpu8086.hpp>

namespace Volt
{
    void CPU8086::Op_PushReg(uint8_t opcode)
    {
        Stack_Push16(*(register_table16[opcode & 0x07]));
    }

    void CPU8086::Op_PopReg(uint8_t opcode)
    {
        *(register_table16[opcode & 0x07]) = Stack_Pop16();
    }

    void CPU8086::Op_PushSegReg(uint8_t opcode)
    {
        Stack_Push16(*segreg_table[opcode >> 3]);
    }

    void CPU8086::Op_PopSegReg(uint8_t opcode)
    {
        // DO NOT FLUSH FOR CS!

        uint8_t index = (opcode >> 3);

        if (index == 0x01)
            Logging_LogChannel("POP CS aka: it's over", LogChannel::Warning);

        *segreg_table[index] = Stack_Pop16();
    }

    void CPU8086::Op_PopModRM(uint8_t opcode)
    {
        CPU8086InstructionModRM modrm = Decode_ModRM(opcode);

        *modrm.ea_ptr = Stack_Pop16();
    }

    void CPU8086::Op_Pushf(uint8_t opcode)
    {
        Stack_Push16(flags);
    }

    void CPU8086::Op_Popf(uint8_t opcode)
    {
        //cleaned up from granite
        uint16_t new_flags = Stack_Pop16();

        //force off bits that exist (FD5 = 0000 1111 1101 0101)
        uint16_t mask = 0xFD5; 

        flags = (new_flags & ~mask) | (new_flags & mask) | 0xF002; 
    }
}
    