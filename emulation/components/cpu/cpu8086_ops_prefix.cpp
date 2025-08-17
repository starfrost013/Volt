// 
// VOLT
// Copyright © 2025 starfrost
//
// cpu8086_ops_prefix.cpp: 8086 prefix ops
//

#include <emulation/emulation.hpp>
#include <emulation/components/cpu/cpu8086.hpp>

namespace Volt
{   
    void CPU8086::Op_DSOverridePrefix(uint8_t opcode)
    {
        seg_override = seg_override_ds;
    }

    void CPU8086::Op_CSOverridePrefix(uint8_t opcode)
    {
        seg_override = seg_override_cs;
    }

    void CPU8086::Op_SSOverridePrefix(uint8_t opcode)
    {
        seg_override = seg_override_ss;
    }

    void CPU8086::Op_ESOverridePrefix(uint8_t opcode)
    {
        seg_override = seg_override_es;
    }
}