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

    class PPI8255 : public ComponentKeyboardController
    {
        
        public: 
            void Init(Machine* machine_ptr) override;
            void Start() override; 
            void Frame() override { }; // Not used here
            void Tick() override { }; // Not used here 
            void Shutdown() override;
        
            uint8_t PortRead(uint8_t port) override;
            void PortWrite(uint8_t port, uint8_t value) override;

            void KeyPress(uint8_t code) override; 

        private:

            enum PPI8255Port
            {
                PortA = 0x60,
                PortB = 0x61,
                PortC = 0x62,
                Cmd = 0x63,
            };

    };
}