// 
// VOLT
// Copyright © 2025 starfrost
//
// component_gpu.hpp: Core GPU functionality
//
#pragma once
#include <emulation/emulation.hpp>

namespace Volt
{
    class VideoMDA : public GPUComponent
    {
        public:
            void Init(Machine* machine_ptr) override { };
            void Start() override { };
            void Tick() override { };
            void Frame() override { };      // Video only
            void Shutdown() override { };

            uint8_t PortRead(uint16_t port) override;
            void PortWrite(uint16_t port, uint8_t value) override;
    }
}