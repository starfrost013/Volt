// 
// VOLT
// Copyright © 2025 starfrost
//
// component_dma_controller.hpp: Implements the most basic form of DMA controller so that other devices can send DMA requests to this device
//
#pragma once
#include <emulation/emulation.hpp>

namespace Volt
{
    class ComponentInterruptController : public Component
    {
    public:
        // It is expected the DMA controller will already be configured
        void Init(Machine* machine_ptr) override { };
        void Start() override { };
        void Tick() override { };
        void Frame() override { };      // Video only
        void Shutdown() override { };

        virtual void SendIRQ(uint8_t irq) = 0; 
    protected:
    private: 
    };
}
