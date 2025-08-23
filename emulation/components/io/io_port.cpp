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
    void IOx86_AddMapping(uint16_t start, uint16_t end, Component* component)
    {
        if (!io_port_range)
            return;

        if (start > end)
        {
            Logging_LogChannel("IOx86_AddMapping: end > start for %s", LogChannel::Debug, component->name);
            return;
        }

        if (end > X86_NUM_IO_PORTS
        || start < 0)
        {
            Logging_LogChannel("IOx86_AddMapping: Port range was out of the 0-%x range for %s!", LogChannel::Debug, end, component->name);
            return;
        }

        for (uint16_t io_port = start; io_port <= end; io_port++)
        {
            io_port_range->entries[io_port].component = component;
        }

        Logging_LogChannel("I/O port range: %04x-%04x registered for device: %s", LogChannel::Debug, start, end, component->name);
    }

    void IOx86_DeleteMapping(uint16_t start, uint16_t end)
    {        
        if (!io_port_range)
            return;

        if (start > end)
        {
            Logging_LogChannel("IOx86_DeleteMapping: start > end", LogChannel::Debug);
            return;
        }

        if (end > X86_NUM_IO_PORTS
        || start < 0)
        {
            Logging_LogChannel("IOx86_DeleteMapping: Port range was out of the 0-%x range for %s!", LogChannel::Debug, end);
            return;
        }

        for (uint16_t io_port = start; io_port < end; io_port++)
        {
            io_port_range->entries[io_port].component = nullptr;  
        }

        Logging_LogChannel("I/O port range: %04x-%04x deleted", LogChannel::Debug, start, end);
    }

    uint8_t IOx86_Read(uint16_t port)
    {
        if (!io_port_range)
            return 0xFF;

        if (!io_port_range->entries[port].component)
        {
            Logging_LogChannel("I/O port %04x read: Nothing here...", LogChannel::Debug, port);
            return 0xFF;
        }

        return io_port_range->entries[port].component->RegisterRead(port);
    }
    
    void IOx86_Write(uint16_t port, uint16_t value)
    {
        if (!io_port_range)
            return;

        if (!io_port_range->entries[port].component)
        {
            Logging_LogChannel("I/O port %04x write value: %04x: Nothing here...", LogChannel::Debug, port, value);
            return;
        }

        io_port_range->entries[port].component->RegisterWrite(port, value);
    }

    void IOx86_Shutdown()
    {
        Memory_Free<IOx86>(io_port_range);
    }
}