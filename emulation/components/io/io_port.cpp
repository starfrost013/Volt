// 
// VOLT
// Copyright © 2025 starfrost
//
// io_port.cpp: I/O implementation for x86 I/O port system
//

#include <emulation/components/io/io_port.hpp>

namespace Volt
{
    IOx86* io_port_range;

    // Initialise the I/O system
    void IOx86_Init()
    {
        io_port_range = Memory_Alloc<IOx86>(TAG_EMU_COMPONENT_IO);
        Logging_LogChannel("IOx86_Init", LogChannel::Debug);
    }

    // Add an I/O mapping
    void IOx86_AddMapping(uint16_t start, uint16_t end, uint8_t (*read)(), void (*write)(uint8_t value), const char* debug_name)
    {
        if (end > start)
        {
            Logging_LogChannel("IOx86_AddMapping: end > start for debug_name %s", LogChannel::Debug, debug_name);
            return;
        }

        for (uint16_t io_port = start; io_port < end; io_port++)
        {
            io_port_range->entries[io_port].read = read;
            io_port_range->entries[io_port].write = write; 
        }

        Logging_LogChannel("I/O port range: %04x-%04x registered for device: %s", LogChannel::Debug, start, end, debug_name);
    }

    void IOx86_DeleteMapping(uint16_t start, uint16_t end)
    {
        if (end > start)
        {
            Logging_LogChannel("IOx86_DeleteMapping: end > start", LogChannel::Debug);
            return;
        }

        for (uint16_t io_port = start; io_port < end; io_port++)
        {
            io_port_range->entries[io_port].read = nullptr;
            io_port_range->entries[io_port].write = nullptr;  
        }

        Logging_LogChannel("I/O port range: %04x-%04x deleted", LogChannel::Debug, start, end);
    }

    void IOx86_Shutdown()
    {

    }
}