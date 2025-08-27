// 
// VOLT
// Copyright © 2025 starfrost
//
// cpu8086_ops_string.cpp: 8086 move ops
//

#include "cpu8086.hpp"
#include <emulation/emulation.hpp>
#include <emulation/components/cpu/cpu8086.hpp>

namespace Volt
{
    void CPU8086::Op_RepPrefix(uint8_t opcode)
    {
        (opcode & 0x01) ? rep_type = CPU8086RepType::Repz : rep_type = CPU8086RepType::Rep;
    }

    void CPU8086::Op_Movs(uint8_t opcode)
    {

    }

    void CPU8086::Op_Cmps(uint8_t opcode)
    {

    }

    void CPU8086::Op_Stos(uint8_t opcode)
    {

    }

    void CPU8086::Op_Lods(uint8_t opcode)
    {

    }

    void CPU8086::Op_Scas(uint8_t opcode)
    {

    }
}