// 
// VOLT
// Copyright © 2025 starfrost
//
// i8255_ppi.hpp: Intel 8255 Programmable Peripheral Interface (System Information & Keyboard)
//

#pragma once
#include <emulation/emulation.hpp>

// BITFIELDS ARE IN REVERSE TO LOGICAL ORDER!

namespace Volt
{
    class PPI8255 : public Component
    {
        void Init(Machine* machine_ptr) override;
        void Start() override; 
        void Frame() override { }; // Not used here
        void Tick() override; 
        void Shutdown() override;
    };
}