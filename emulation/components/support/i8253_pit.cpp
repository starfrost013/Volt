// 
// VOLT
// Copyright © 2025 starfrost
//
// i8253_pit.cpp: Implementation for i8253 Programmable Interval Timer chip
//

#include <emulation/components/support/i8253_pit.hpp>

namespace Volt
{
    Cvar* emu_clk_8253;

    void PIT8253::Init()
    {

        //TEMP
        variant = PIT8253Variant::PITVariant8253;

        switch (variant)
        {
            case PIT8253Variant::PITVariant8253:
                name = "Intel 8253 PIT";
                break;
            case PIT8253Variant::PITVariant8254:
                name = "Intel 8254 PIT";
                break; 
        }

        emu_clk_8253 = Cvar_Get("emu_clk_8253", "1193181", false);
        clock_hz = emu_clk_8253->value;

        IOx86_AddMapping(0x40, 0x43, this);

        Logging_LogChannel("Intel 8253/54 PIT: Clockspeed = %d Hz", LogChannel::Debug, clock_hz);
    }

    void PIT8253::Start() 
    {

    }
    
    void PIT8253::Tick()
    {

    }

    void PIT8253::Shutdown()
    {  
        IOx86_DeleteMapping(0x40, 0x43);
    }

    uint8_t PIT8253::RegisterRead(uint8_t port)
    {
        Logging_LogChannel("Intel 8253/54 PIT port %04x read *placeholder*", LogChannel::Debug, port);
        return 0x00; 
    }

    void PIT8253::RegisterWrite(uint8_t port, uint8_t value)
    {
        Logging_LogChannel("Intel 8253/54 PIT port %04x write, value %04x", LogChannel::Debug, port, value);

    }
}