// 
// VOLT
// Copyright © 2025 starfrost
//
// i8237_dma.hpp: Intel 8237 DMA controller definition 
//

#pragma once
#include <emulation/emulation.hpp>

// BITFIELDS ARE IN REVERSE TO LOGICAL ORDER!

namespace Volt
{
    class DMA8237 : public Component
    {
        public: 
            // Later boxes have 2x 8237s
            bool is_secondary;

            void Init() override;
            void Start() override; 
            void Frame() override { }; // Not used here
            void Tick() override; 
            void Shutdown() override;
        }
}