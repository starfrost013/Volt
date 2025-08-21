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
        stack_push_16(*(register_table16[opcode & 0x07]));
    }

    void CPU8086::Op_PopReg(uint8_t opcode)
    {
        *(register_table16[opcode & 0x07]) = stack_pop_16();
    }

    void CPU8086::Op_PushSegReg(uint8_t opcode)
    {
        stack_push_16(*segreg_table[opcode >> 3]);
    }

    void CPU8086::Op_PopSegReg(uint8_t opcode)
    {
        // DO NOT FLUSH FOR CS!

        uint16_t segreg = *segreg_table[opcode >> 3];

        if (segreg == 0x01)
            Logging_LogChannel("POP CS aka: it's over", LogChannel::Warning);

        segreg = stack_pop_16();
    }
}