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
    class Machine; 
    
    class Component
    {
    public:
        const char* name;
        Machine* machine;                           // don't store this if you don't need to lol

        // Clocking
        uint64_t clock_hz;
        uint64_t clock_skip;
        double microseconds_current;
        double microseconds_target;

        // Does this component need to be updated? 
        // A CPU needs to be ticked.
        // A printer card does not need to be ticked.
        bool update; 
        bool allow_device_reads;                    // If false, this device is write only. E.g. EGA
        bool allow_device_writes;                   // If false, this device is read only. E.g. ROM
        bool early_start;                           // Start this device early.

        virtual void Init(Machine* machine_ptr) = 0;
        virtual void Start() = 0;
        virtual void Tick() = 0;
        virtual void Frame() = 0;       // Video only
        virtual void Shutdown() = 0;

        virtual uint8_t PortRead(uint16_t port) { return 0x00; };
        virtual void PortWrite(uint16_t port, uint8_t value) { };

        virtual uint8_t MemoryReadByte(uint32_t pos) { return 0x00; };
        virtual uint16_t MemoryReadWord(uint32_t pos) { return 0x00; };
        virtual uint32_t MemoryReadDword(uint32_t pos) { return 0x00; };

        virtual void MemoryWriteByte(uint32_t pos, uint8_t value) { };
        virtual void MemoryWriteWord(uint32_t pos, uint16_t value) { };
        virtual void MemoryWriteDword(uint32_t pos, uint32_t value) { };

        Component()
        {

        }
    protected:
    private: 
    };
}
