// 
// VOLT
// Copyright © 2025 starfrost
//
// i8259_pic.cpp: Intel 8259 Programmable Interrupt Controller implementation
//

#include <emulation/components/support/i8259_pic.hpp>

namespace Volt
{
    void PIC8259::Init(Machine* machine_ptr)
    {
        machine = machine_ptr;
        name = "Intel 8259 Programmable Interrupt Controller";
    }

    void PIC8259::Start()
    {

    }

    void PIC8259::Tick()
    {

    }

    void PIC8259::Shutdown() 
    {
        
    }

    void PIC8259::SendIRQ(uint8_t irq)
    {
        
    }
}