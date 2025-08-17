// 
// VOLT
// Copyright © 2025 starfrost
//
// cpu8086_ops_math.cpp: 8086 math ops
//

#include <emulation/emulation.hpp>
#include <emulation/components/cpu/cpu8086.hpp>

namespace Volt
{
    void CPU8086::Op_Inc(uint8_t opcode)
    {
        (*register_table16[opcode & 0x07])++;

        uint16_t result = *register_table16[opcode & 0x07];

        SetPZSFlags16(result);
        // Investigate functions to set these. A lot of them, however, are kind of one liner

        if (result & 0x0F == 0x00) 
            flags |= CPU8086Flags::AuxCarry;

        SetOF16_Add(result, result - 1, 1);
    }

    void CPU8086::Op_Dec(uint8_t opcode)
    {
        (*register_table16[opcode & 0x07])--;

        uint16_t result = *register_table16[opcode & 0x07];

        SetPZSFlags16(result);

        if (result & 0x0F == 0x0F) 
            flags |= CPU8086Flags::AuxCarry;

        SetOF16_Dec(result, result - 1, 1);
    }
    
}