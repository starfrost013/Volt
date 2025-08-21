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
}