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
    class VideoMDA : public ComponentGPU
    {
    public:
        void Init(Machine* machine_ptr);
        void Start();
        void Tick();
        void Frame();    
        void Shutdown();

        uint8_t PortRead(uint16_t port);
        void PortWrite(uint16_t port, uint8_t value);
    };
}