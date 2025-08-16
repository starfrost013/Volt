// 
// VOLT
// Copyright © 2025 starfrost
//
// cpu8086_core.cpp: 8086 control ops
//

#include <emulation/emulation.hpp>
#include <emulation/components/cpu/cpu8086.hpp>

namespace Volt
{
    void CPU8086::Op_JmpFar()
    {
        uint16_t new_ip = Prefetch_Pop16();
        uint16_t new_cs = Prefetch_Pop16();
    
        Logging_LogChannel("JMPF %04x:%04x", LogChannel::Debug, new_cs, new_ip);
    
        cs = new_cs;
        ip = new_ip;
    }
}