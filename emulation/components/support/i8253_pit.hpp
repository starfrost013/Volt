// 
// VOLT
// Copyright © 2025 starfrost
//
// cpu8086.hpp: Generic 808x-class CPU (8086, 8088, V30)
//

#pragma once
#include <emulation/emulation.hpp>

// BITFIELDS ARE IN REVERSE TO LOGICAL ORDER!

namespace Volt
{
    #define PIT8253_NUM_COUNTERS        3   



    class PIT8253 : public Component
    {
        public:

            enum PIT8253Variant
            {
                PITVariant8253 = 0x8253,
                PITVariant8254 = 0x8254,
            };

            PIT8253Variant variant;

            uint16_t control;

            uint16_t counters[PIT8253_NUM_COUNTERS];

            void Init() override;
            void Start() override; 
            void Frame() override { }; // Not used here
            void Tick() override; 
            void Shutdown() override;

            uint8_t RegisterRead(uint8_t port) override;
            void RegisterWrite(uint8_t port, uint8_t value) override;

        protected:
        private:
    };

}