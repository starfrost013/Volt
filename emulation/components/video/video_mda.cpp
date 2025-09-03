// 
// VOLT
// Copyright © 2025 starfrost
//
// video_mda.cpp: MDA Video Card implementation
//

#include <emulation/components/video/video_mda.hpp>

namespace Volt
{
    Cvar* emu_mda_clock;

    void VideoMDA::Init(Machine* machine_ptr)
    {
        emu_mda_clock = Cvar_Get("emu_mda_clock", "4772726", false);

        machine = machine_ptr;
        name = "IBM Monochrome Display Adapter";

        IOx86_AddMapping(0x3B0, 0x3BF, this);
    }

    void VideoMDA::Start()
    {

    }

    void VideoMDA::Tick()
    {

    }

    void VideoMDA::Shutdown() 
    {
        
    }

    uint8_t PortRead(uint16_t port)
    {
        Logging_LogChannel("IBM MDA read port %04x", LogChannel::Debug, port);
    }

    void PortWrite(uint16_t port, uint8_t value)
    {
        Logging_LogChannel("IBM MDA write port %04x, value %02X", LogChannel::Debug, port, value);
    }
}