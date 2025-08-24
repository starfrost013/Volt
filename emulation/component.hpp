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

        // Clocking
        uint64_t clock_hz;
        uint64_t clock_skip;
        double microseconds_current;
        double microseconds_target;

        // Does this component need to be updated? 
        // A CPU needs to be ticked.
        // A printer card does not need to be ticked.
        bool update; 
        
        virtual void Init() = 0;
        virtual void Start() = 0;
        virtual void Tick() = 0;
        virtual void Frame() = 0;       // Video only
        virtual void Shutdown() = 0;

        virtual uint8_t PortRead(uint8_t port) { return 0x00; };
        virtual void PortWrite(uint8_t port, uint8_t value) { };

        Component()
        {

        }
    protected:
    private: 
    };
}
