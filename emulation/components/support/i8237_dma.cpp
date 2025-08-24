// 
// VOLT
// Copyright © 2025 starfrost
//
// i8237_dma.hpp: Intel 8237 DMA controller definition 
//

#include <emulation/components/support/i8237_dma.hpp>

// BITFIELDS ARE IN REVERSE TO LOGICAL ORDER!

namespace Volt
{ 
    Cvar* emu_8237_clk;

    void DMA8237::Init()
    {
        name = "Intel 8237 DMA controller";
        emu_8237_clk = Cvar_Get("emu_8237_clk", "4772726", false);

        IOx86_AddMapping(0x00, 0x0F, this);         // 8237 #1  
        IOx86_AddMapping(0xC0, 0xCF, this);         // 8237 #2 - Word
        IOx86_AddMapping(0xD0, 0xDF, this);         // 8237 #2 - Byte
    }
    
    void DMA8237::Start()
    {
        // TODO: if issues aries, we might have to configure master/slave stuff here
    }

    void DMA8237::Tick()
    {

    }

    uint8_t DMA8237::PortRead(uint8_t port) 
    {
        bool is_channel_4to7 = (port >= 0xC0);
        bool w = true;

        if (is_channel_4to7)
            w = !(port >= 0xD0);

        // convert to a port number
        port &= 0x0F; 
        
    }

    void DMA8237::PortWrite(uint8_t port, uint8_t value) 
    {

    }

    void DMA8237::Shutdown()
    {
        IOx86_DeleteMapping(0x00, 0x0F);
        IOx86_DeleteMapping(0xC0, 0xCF);
        IOx86_DeleteMapping(0xD0, 0xDF);
    }
}