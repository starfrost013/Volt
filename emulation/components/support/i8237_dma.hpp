// 
// VOLT
// Copyright © 2025 starfrost
//
// i8237_dma.hpp: Intel 8237 DMA controller definition 
// Note: We emulate all 7 channels that an AT-based setup would have. The single-8237 PC and XT setup ignores channels 4 through 7 anyway.
//
// A big part of these was never used on the PC

#pragma once
#include <emulation/emulation.hpp>

// BITFIELDS ARE IN REVERSE TO LOGICAL ORDER!

namespace Volt
{
    // Technically two 8237s
    #define DMA8237_NUM_CHANNELS        8
 

    class DMA8237 : public Component
    {
        public: 
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
            };

            enum DMA8237XferState
            {
                NotPending = 0,
                Pending = 1,
                InProgress = 2,
                Complete = 3,
            };

            enum DMA8237AddressMode
            {
                Increment = 0,
                Decrement = 1,
            };

            struct DMA8237Channel
            {
                DMA8237Mode mode; 
                DMA8237Direction direction;
                DMA8237XferState xfer_state; 
                DMA8237AddressMode address_mode; 

                uint16_t start_addr; 
                uint16_t transfer_count;
            }; 

            DMA8237Channel channels[DMA8237_NUM_CHANNELS];

            void Init() override;
            void Start() override; 
            void Frame() override { }; // Not used here
            void Tick() override; 
            void Shutdown() override;

            uint8_t PortRead(uint8_t port) override;
            void PortWrite(uint8_t port, uint8_t value) override;
        };
}