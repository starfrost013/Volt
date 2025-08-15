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
        //add primary address space
        address_space = AddressSpace_Add<CPU8086_ADDR_SPACE_SIZE>(); 
    }

    void CPU8086::Tick()
    {
        //TODO: PREFETCH QUEUE IMPLEMENTATION
        uint8_t opcode = address_space->access_byte[linear_pc];

        if (seg_override != seg_current)
            seg_current = seg_override; 

        if (instruction_table[opcode].run_function)
            (this->*instruction_table[opcode].run_function)();

        ip += instruction_table[opcode].size;

        ip &= 0xFFFF; // wrap around
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