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
    class ComponentGPU : public Component
    {
    public:
        void Init(Machine* machine_ptr) override { };
        void Start() override { };
        void Tick() override { };
        void Frame() override { };      // Video only
        void Shutdown() override { };
    protected:
    private:
    };
}