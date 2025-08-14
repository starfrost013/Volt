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

    }

    void CPU8086::Frame()
    {
        //not used for CPUs. Only for some video cards...
    }

    void CPU8086::Shutdown()
    {

    }
}