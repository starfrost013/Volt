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

            enum PIT8253Port
            {
                Ch0 = 0x40,
                Ch1 = 0x41,
                Ch2 = 0x42,
                ControlWord = 0x43,
            };

            enum PIT8253Ctrl
            {
                BCD = 1,
                OperatingMode = 1 << 1,
                AccessMode = 1 << 4,
                ChannelSelect = 1 << 6,
            };

            enum PIT8253Mode
            {
                IntOnTerminal = 0,
                OneShot = 1,
                RateGenerator = 2,
                SquareWave = 3,
                StrobeSoftware = 4,
                StrobeHardware = 5,
            };

            PIT8253Variant variant;

            struct PIT8253Counter
            {
                uint16_t value;
                uint8_t control;                    // The last control value that was saved for this particular counter channel
                uint16_t value_on_latch;            // Value on latch command 
                uint16_t value_on_reload;           // Value on last reload request for mode 2/3
                uint8_t latch_state;                // counter so we know what to do for reads
                bool write_2nd_byte;             
                bool read_2nd_byte;                 // should we *read* the 2nd byte? a separate bool so that it does not conflict with the write
                bool output;                        // clock output
                bool running;                       // is this running?
                
            };

            PIT8253Counter counters[PIT8253_NUM_COUNTERS];

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