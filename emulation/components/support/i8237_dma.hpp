// 
// VOLT
// Copyright © 2025 starfrost
//
// i8237_dma.hpp: Intel 8237 DMA controller definition 
// Note: In order to emulate all channels, add two of these. Before init, but after creating the object, set "is_channels_4to7" to true 
// The single-8237 PC and XT setup ignores channels 4 through 7 anyway.
//
// A big part of these was never used on the PC

#pragma once
#include <emulation/emulation.hpp>

// BITFIELDS ARE IN REVERSE TO LOGICAL ORDER!

namespace Volt
{
    // Technically two 8237s
    #define DMA8237_NUM_CHANNELS        4
 

    class DMA8237 : public Component
    {
        public: 
            void Init() override;
            void Start() override; 
            void Frame() override { }; // Not used here
            void Tick() override; 
            void Shutdown() override;

            uint8_t PortRead(uint8_t port) override;
            void PortWrite(uint8_t port, uint8_t value) override;

        private:
            // Mode for DMA transfers
            enum DMA8237Mode
            {
                OnDemand = 0,
                Single = 1,
                Block = 2,
                Cascade = 3,
            };

            enum DMA8237Direction
            {
                Verify = 0,
                ToMemory = 1,
                FromMemory = 2,
                Illegal = 3,
            };


            enum DMA8237AddressMode
            {
                Increment = 0,
                Decrement = 1,
            };

            enum DMA8237Register
            {
                Status = 0x08,      // Status on read
                Command = 0x08,     // Command on write
                WriteReq = 0x09,
                ChannelMask = 0x0A,
                Mode = 0x0B,
                FlipFlop = 0x0C,
                Temp = 0x0D,        // Read
                MasterClear = 0x0D, // Write
                ClearMask = 0x0F, 
            };

            enum DMA8237Command
            {
                MemToMem = 1,
                DisableChannel0Hold = 1 << 1,
                Disable = 1 << 2,
                CompressTiming = 1 << 3,
                RotatingPriority = 1 << 4,          // 0 = fixed
                ExtendedWriteSelect = 1 << 5,       // Ignore if timing compresed. 0 = fast
                DreqActive = 1 << 6,                // 0 = high, 1 = low
                DackActive = 1 << 7,                // 0 = high, 1 = low
            };

            struct DMA8237Channel
            {
                DMA8237Mode mode; 
                DMA8237Direction direction;
                DMA8237AddressMode address_mode; 

                uint16_t start_addr; 
                uint16_t transfer_count;
                uint16_t word_count; 
                uint8_t request_info;

                bool tc_reached;        // Has the terminal count been reached?

                bool request_pending;
                bool masked;            // turn this channel off
            }; 

            uint8_t status_reg;
            uint8_t command_reg;
            uint8_t temp_reg;

            bool flipflop;

            DMA8237Channel channels[DMA8237_NUM_CHANNELS];

            bool is_channels_4to7;
            bool enabled;               // set bit2 in command register
        };
}