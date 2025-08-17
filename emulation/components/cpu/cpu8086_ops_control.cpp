// 
// VOLT
// Copyright © 2025 starfrost
//
// cpu8086_ops_control.cpp: 8086 control flow ops
//

#include <emulation/emulation.hpp>
#include <emulation/components/cpu/cpu8086.hpp>

namespace Volt
{
    void CPU8086::Op_JmpFar(uint8_t opcode)
    {
        uint16_t new_ip = Prefetch_Pop16();
        uint16_t new_cs = Prefetch_Pop16();

        cs = new_cs;
        ip = new_ip;

        Logging_LogChannel("JMPF %04x:%04x", LogChannel::Debug, new_cs, new_ip);

        // flush the prefetch queue
        Prefetch_Flush(); 
    }
}