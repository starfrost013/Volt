// 
// VOLT
// Copyright © 2025 starfrost
//
// component_keyboard_controller.hpp: Implements the most basic form of keyboard controller so that other devices can send keyboard/dip requests to this device
// In the future, we'll emulate the 804x
//

#pragma once
#include <emulation/emulation.hpp>// 

namespace Volt
{
    class ComponentKeyboardController : public Component
    {
    public:
        // It is expected the DMA controller will already be configured
        void Init(Machine* machine_ptr) override { };
        void Start() override { };
        void Tick() override { };
        void Frame() override { };      // Video only
        void Shutdown() override { };

        virtual void KeyPress(uint8_t code) = 0; 
    protected:
    private: 
    };
}
