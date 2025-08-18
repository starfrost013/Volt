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
        {
            *register_table16[opcode % 0x08] = Prefetch_Pop16();
        }
        else
        {
            *register_table8[opcode % 0x08] = Prefetch_Pop8();
        }
    }
}