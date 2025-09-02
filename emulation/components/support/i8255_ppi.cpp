
// 
// VOLT
// Copyright © 2025 starfrost
//
// i8255_ppi.cpp: Intel 8255 Programmable Peripheral Interface (System Information & Keyboard) implementation
// A lot of this is TODO. The reason is that it is dependent on system configuration
//

#include <emulation/components/support/i8255_ppi.hpp>

namespace Volt
{
    void PPI8255::Init(Machine* machine_ptr)
    {
        machine = machine_ptr;
        name = "Intel 8255 Programmable Peripheral Interface";

        IOx86_AddMapping(0x60, 0x63, this);

        update = false; 
    }

    void PPI8255::Start()
    {

    }

    void PPI8255::Shutdown() 
    {

    }

    uint8_t PPI8255::PortRead(uint16_t port) 
    {
        uint8_t ret = 0x00;

        switch (port)
        {
            default:
                ret = 0xFF; 
                break; 
            case PPI8255Port::PortC:
                ret = 0x00;
                break; 
        }

        Logging_LogChannel("8255 PPI read port %04X value %02x", LogChannel::Debug, port, ret);
        return ret; 
    }

    void PPI8255::PortWrite(uint16_t port, uint8_t value)
    {
        Logging_LogChannel("8255 PPI write port %04X value %02x", LogChannel::Debug, port, value);
    }

    void PPI8255::KeyPress(uint8_t code) 
    {
        
    }

}