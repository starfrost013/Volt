// 
// VOLT
// Copyright © 2025 starfrost
//
// i8259_pic.hpp: Intel 8259 Programmable Interrupt Controller definition
//

#pragma once
#include <emulation/emulation.hpp>

// BITFIELDS ARE IN REVERSE TO LOGICAL ORDER!

namespace Volt
{
    class PIC8259 : public ComponentInterruptController
    {
        void Init(Machine* machine_ptr) override;
        void Start() override; 
        void Frame() override { }; // Not used here
        void Tick() override; 
        void Shutdown() override;

        void SendIRQ(uint8_t irq) override; 

    };
}