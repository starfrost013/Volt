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
    Cvar* emu_8237_clock;

    void DMA8237::Init(Machine* machine_ptr)
    {
        machine = machine_ptr;

        name = (is_channels_4to7) ? "Intel 8237 DMA controller (Slave)" : "Intel 8237 DMA controller (Master)";
        
        emu_8237_clock = Cvar_Get("emu_8237_clock", "4772726", false);

        //only map 1 each unless they are continguous
        IOx86_AddMapping(0x81, 0x83, this);
        IOx86_AddMapping(0x87, 0x87, this);         // why is channel 0 here

        if (is_channels_4to7)
        {
            IOx86_AddMapping(0xC0, 0xCF, this);         // 8237 #2 - Word
            IOx86_AddMapping(0xD0, 0xDF, this);         // 8237 #2 - Byte
        }
        else
        {
            IOx86_AddMapping(0x00, 0x0F, this);         // 8237 #1  
        }
    }
    
    void DMA8237::Start()
    {
        // TODO: if issues aries, we might have to configure master/slave stuff here
    }

    uint8_t DMA8237::Tick_Read(uint8_t channel)
    {
        return 0x00;
    }

    void DMA8237::Tick_Write(uint8_t channel, uint8_t value)
    {
        Logging_LogChannel("TODO: DMA 8237 Write\n", LogChannel::Warning);
    }

    void DMA8237::Tick()
    {
        for (uint32_t channel_id = 0; channel_id < DMA8237_NUM_CHANNELS; channel_id++)
        {
            DMA8237Channel& channel = channels[channel_id];

            //see if a transfer has been requested
            switch (channel.transfer_type)
            {
                case DMA8237XferType::Single:
                    switch (channel.direction)
                    {
                        case DMA8237Direction::Illegal:
                            Logging_LogChannel("8237 DMA Controller invalid transfer direction!!!!\n", LogChannel::Warning);
                            break;
                        case DMA8237Direction::Verify:
                        case DMA8237Direction::FromMemory:
                            
                            break;
                    }
            }
        }
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
                        ret = (channel.word_count >> 8) & 0xFF;
                    else
                        ret = (channel.word_count) & 0xFF;
                }
                else
                {
                    if (flipflop)
                        ret = (channel.start_addr >> 8) & 0xFF;
                    else
                        ret = (channel.start_addr) & 0xFF;
                }

                // Flop it!
                flipflop = !flipflop;
                break;
            case DMA8237Register::Status:
                for (uint32_t channel_id = 0; channel_id < DMA8237_NUM_CHANNELS; channel_id++)
                {
                    // bits 3-0 = channel[bit] is done
                    // bits 7-4 = channel[bit] wants service

                    channel = channels[channel_id];

                    if (channel.tc_reached)
                        ret |= (1 << channel_id);
                    
                    if (channel.request_pending)
                        ret |= (1 << (channel_id + 4));
                }
                break;
            case DMA8237Register::Temp:
                ret = temp_reg;
                break;
                
            // these are stupid and have weird gaps
            case DMA8237Register::PageChannel0:
            case DMA8237Register::PageChannel1:
            case DMA8237Register::PageChannel2:
            case DMA8237Register::PageChannel3:
                if (port == DMA8237Register::PageChannel0)
                    channel = channels[0];
                else
                    channel = channels[port & 0x03];
                break;

                ret = channel.page; 
        }
        
        Logging_LogChannel("DMA8237 Read port %02x value %02x", LogChannel::Debug, port, ret);

        return ret; 
    }

    void DMA8237::PortWrite(uint8_t port, uint8_t value) 
    {
        bool is_channel_4to7 = (port >= 0xC0);
        bool w = true;
        uint8_t ret = 0x00;

        uint8_t channel_number = value & 0x03;
        
        if (is_channel_4to7)
        {
            w = !(port >= 0xD0);
            channel_number |= 4;
        }

        DMA8237Channel& channel = channels[channel_number]; // gets hcanged later

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
                        channel.word_count = ((channel.word_count >> 8) << 8) | value;
                }
                else //base address
                {
                    if (flipflop)
                        channel.start_addr = (channel.start_addr & 0xFF) | (value << 8);
                    else
                        channel.start_addr = ((channel.start_addr >> 8) << 8) | value;

                }

                flipflop = !flipflop;
                break;
            // not all of this even works on the PC
            case DMA8237Register::Command:
                command_reg = value;

                enabled = !(command_reg & DMA8237Command::Disable);

                if (!enabled)
                    Logging_LogChannel("ISA 8237 DMA disabled", LogChannel::Debug);
                else
                    Logging_LogChannel("ISA 8237 DMA enabled", LogChannel::Debug);

                break;
            case DMA8237Register::ChannelMask:
                channel.masked = (value & 0x04) != 0;
                break;
            case DMA8237Register::WriteReq:
                // figure out the channel we are trying to write#

                // Software DMA
                channel.request_pending = (value >> 2) & 0x01; 

                break;
            case DMA8237Register::Mode: // set up mode information
                channel.direction = static_cast<DMA8237Direction>((value >> 2) & 0x03);
                channel.autoinit = (value & (1 << 4));
                channel.address_mode = static_cast<DMA8237AddressMode>((value >> 5) & 0x01);
                channel.transfer_type = static_cast<DMA8237XferType>((value >> 6) & 0x03);

                channel.tc_reached = false; 
                break;
            case DMA8237Register::MasterClear:
                for (uint32_t channel_id = 0; channel_id < DMA8237_NUM_CHANNELS; channel_id++)
                    channels[channel_id].masked = true;
                
                command_reg = status_reg = temp_reg = 0x00;
                flipflop = false;

                Logging_LogChannel("DMA8237 Reset", LogChannel::Debug);
                break;  
            case DMA8237Register::ClearMask:
                for (uint32_t channel_id = 0; channel_id < DMA8237_NUM_CHANNELS; channel_id++)
                    channels[channel_id].masked = true;
                break; 
            case DMA8237Register::WriteMask:
                for (uint32_t channel_id = 0; channel_id < DMA8237_NUM_CHANNELS; channel_id++)
                    channels[channel_id].masked = (value >> channel_id) & 0x01;
                break;
            // these are stupid and have weird gaps
            case DMA8237Register::PageChannel0:
            case DMA8237Register::PageChannel1:
            case DMA8237Register::PageChannel2:
            case DMA8237Register::PageChannel3:
                if (port == DMA8237Register::PageChannel0)
                    channel = channels[0];
                else
                    channel = channels[port & 0x03];

                channel.page = value; 
                break;
        }

        channels[channel_number] = channel;

        Logging_LogChannel("DMA8237 Write port %02x value %02x", LogChannel::Debug, port, value);
    }

    void DMA8237::Shutdown()
    {
        IOx86_DeleteMapping(0x00, 0x0F);
        IOx86_DeleteMapping(0xC0, 0xCF);
        IOx86_DeleteMapping(0xD0, 0xDF);
    }

    void DMA8237::SendDMARequest(uint8_t channel) 
    {
        Logging_LogChannel("8237: DMA request initiated by another system device for channel %d [TODO]", LogChannel::Debug, channel);
    }

}