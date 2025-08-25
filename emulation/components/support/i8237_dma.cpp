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
        name = (is_channels_4to7) ? "Intel 8237 DMA controller (Slave)" : "Intel 8237 DMA controller (Master)";
        
        emu_8237_clk = Cvar_Get("emu_8237_clk", "4772726", false);

        IOx86_AddMapping(0x00, 0x0F, this);         // 8237 #1  

        if (is_channels_4to7)
        {
            IOx86_AddMapping(0xC0, 0xCF, this);         // 8237 #2 - Word
            IOx86_AddMapping(0xD0, 0xDF, this);         // 8237 #2 - Byte
        }

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
        uint8_t ret = 0x00;
        uint8_t channel_number;
        
        if (is_channel_4to7)
            w = !(port >= 0xD0);

        // convert to a port number
        port &= 0x0F; 
        DMA8237Channel channel;

        switch (port)
        {
            default:
                channel_number = (port >> 1);
                channel = channels[channel_number];

                if (is_channel_4to7)
                    channel_number |= 4;
                
                // Word Count
                if (port & 0x01)
                {
                    if (flipflop)
                        ret = (channel.start_addr >> 8) & 0xFF;
                    else
                        ret = (channel.start_addr) & 0xFF;
                }
                else
                {
                    if (flipflop)
                        ret = (channel.word_count >> 8) & 0xFF;
                    else
                        ret = (channel.word_count) & 0xFF;
                }

                // Flop it!
                flipflop = !flipflop;
                break;
            case DMA8237Register::Status:
                for (uint32_t i = 0; i < DMA8237_NUM_CHANNELS; i++)
                {
                    // bits 3-0 = channel[bit] is done
                    // bits 7-4 = channel[bit] wants service

                    channel = channels[i];

                    if (channel.tc_reached)
                        ret |= (1 << i);
                    
                    if (channel.request_pending)
                        ret |= (1 << (i + 4));
                }
                break;
        }
        
        return ret; 
    }

    void DMA8237::PortWrite(uint8_t port, uint8_t value) 
    {
        bool is_channel_4to7 = (port >= 0xC0);
        bool w = true;
        uint8_t ret = 0x00;

        if (is_channel_4to7)
            w = !(port >= 0xD0);

        uint8_t channel_number;
        DMA8237Channel& channel = channels[0]; // gets hcanged later

        switch (port)
        {
            default:
                channel_number = (port >> 1);
                channel = channels[channel_number];

                if (is_channel_4to7)
                    channel_number |= 4;

                //word count
                if (port & 0x01)
                {
                    if (flipflop)
                        channel.word_count = (channel.word_count & 0xFF) | (value << 8);
                    else
                        channel.word_count |= value;
                }
                else //base address
                {
                    if (flipflop)
                        channel.start_addr = (channel.start_addr & 0xFF) | (value << 8);
                    else
                        channel.start_addr |= value;

                }

                flipflop = !flipflop;
                break;
            // not all of this even works on the PC
            case DMA8237Register::Command:
                enabled = (command_reg & DMA8237Command::Disable);

                if (!enabled)
                    Logging_LogChannel("ISA 8237 DMA disabled", LogChannel::Debug);
                else
                    Logging_LogChannel("ISA 8237 DMA enabled", LogChannel::Debug);

                command_reg = value;
                break;
                
        }
    }

    void DMA8237::Shutdown()
    {
        IOx86_DeleteMapping(0x00, 0x0F);
        IOx86_DeleteMapping(0xC0, 0xCF);
        IOx86_DeleteMapping(0xD0, 0xDF);
    }
}