// 
// VOLT
// Copyright © 2025 starfrost
//
// io_port.hpp: Class definition for x86 I/O port system
//

#pragma once
#include <emulation/emulation.hpp>

namespace Volt
{
    #define X86_NUM_IO_PORTS        65536

    // I/O entry
    // Consider if we should have a two level lookup or not
    // Using an array is faster than std::unordered_map
    // Devices that need 16 or 32 bit reads/writes can use contiguous ports and convert values as needed.
    struct IOx86Entry
    {
        uint8_t (*read)();
        void (*write)(uint8_t value);
    };

    struct IOx86
    {
        IOx86Entry entries[X86_NUM_IO_PORTS];
    };

    extern IOx86* io_port_range;

    void IOx86_Init();
    void IOx86_AddMapping(uint16_t start, uint16_t end, uint8_t (*read)(), void (*write)(uint8_t value), const char* debug_name);
    void IOx86_DeleteMapping(uint16_t start, uint16_t end);
    uint8_t IOx86_Read(uint16_t port);
    void IOx86_Write(uint16_t port, uint16_t value);
    void IOx86_Shutdown();
}