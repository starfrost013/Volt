// 
// VOLT
// Copyright © 2025 starfrost
//
// cpu8086_core.cpp: 8086 core implementation
//
#include "cpu8086.hpp"
#include <emulation/emulation.hpp>
#include <emulation/components/cpu/cpu8086.hpp>

namespace Volt
{

    void CPU8086::Init()
    {
        // cannot use constructor here due to MemAlloc limitations
        if (!variant)
            variant = CPU8086Variant::cpu808x_8086;

        switch (variant)
        {
            case CPU8086Variant::cpu808x_8086:
                name = "[1978] Intel 8086 CPU";
                break;
            case CPU8086Variant::cpu808x_8088:
                name = "[1979] Intel 8088 CPU";
                break; 
            default:
                name = "Intel 808x Family CPU";
                break;
        }

        Logging_LogChannel("8086: Initialising address space (variant: %s)", LogChannel::Debug, name);

        //add primary address space
        address_space = AddressSpace_Add<CPU8086_ADDR_SPACE_SIZE>();
        
        update = true; 
    }

    void CPU8086::Tick()
    {
        //TODO: PREFETCH QUEUE IMPLEMENTATION
        uint8_t opcode = address_space->access_byte[linear_pc];

        
        if (instruction_table[opcode].run_function)
            (this->*instruction_table[opcode].run_function)();

        ip += instruction_table[opcode].size;

        ip &= 0xFFFF; // wrap around
   
        //if we reached here there are no more segment prefixes
        // so reset
        seg_override = seg_override_none;
    }

    void CPU8086::Frame()
    {
        //not used for CPUs. Only for some video cards...
    }

    void CPU8086::Shutdown()
    {

    }

    //
    // OPcodes (move to own file)
    //

    void CPU8086::Op_Nop()
    {

    }
}