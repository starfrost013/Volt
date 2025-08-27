
// 
// VOLT
// Copyright © 2025 starfrost
//
// i8255_ppi.cpp: Intel 8255 Programmable Peripheral Interface (System Information & Keyboard) implementation
//

#include <emulation/components/support/i8255_ppi.hpp>

namespace Volt
{
    void PPI8255::Init(Machine* machine_ptr)
    {
        machine = machine_ptr;
        name = "Intel 8255 Programmable Peripheral Interface";
    }

    void PPI8255::Start()
    {

    }

    void PPI8255::Tick()
    {

    }

    void PPI8255::Shutdown() 
    {

    }
}