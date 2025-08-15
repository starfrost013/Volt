// 
// VOLT
// Copyright © 2025 starfrost
//
// component.hpp: The class for a machine component
//
#pragma once
#include <emulation/emulation.hpp>

namespace Volt
{
    class Component
    {
    public:
        const char* name;
        uint64_t clock_hz;
        uint64_t clock_skip;

        // Does this component need to be updated? 
        // A CPU needs to be ticked.
        // A printer card does not need to be ticked.
        bool update; 
        
        virtual void Init() = 0;
        virtual void Tick() = 0;
        virtual void Frame() = 0;       // Video only
        virtual void Shutdown() = 0;

        Component()
        {

        }
    protected:
    private: 
    };
}
