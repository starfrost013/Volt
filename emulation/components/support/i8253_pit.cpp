// 
// VOLT
// Copyright © 2025 starfrost
//
// i8253_pit.cpp: Implementation for i8253 Programmable Interval Timer chip
//

#include <emulation/components/support/i8253_pit.hpp>

namespace Volt
{
    Cvar* emu_8253_clk;

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

        emu_8253_clk = Cvar_Get("emu_8253_clk", "1193181", false);
        clock_hz = emu_8253_clk->value;
        update = true; 

        IOx86_AddMapping(0x40, 0x43, this);

        Logging_LogChannel("Intel 8253/54 PIT: Clockspeed = %d Hz", LogChannel::Debug, clock_hz);
    }

    void PIT8253::Start() 
    {

    }
    
    void PIT8253::Tick()
    {
        for (uint32_t counter_id = 0; counter_id < PIT8253_NUM_COUNTERS; counter_id++)
        {
            PIT8253Counter& current_counter = counters[counter_id];

            uint8_t control = current_counter.control;

            if (!current_counter.running)
                continue; 

            PIT8253Mode mode = static_cast<PIT8253Mode>((control >> 1) & 0x07); 

            switch (mode)
            {  
                case PIT8253Mode::IntOnTerminal:
                    current_counter.value--; 

                    if (!current_counter.value && !current_counter.output)
                    {
                        Logging_LogChannel("PIC IRQ0 must be here, but isn't yet [PIT8253 Counter %d interrupt on Terminate zero value reached!]\n", LogChannel::Warning, counter_id);    
                        current_counter.output = true; 
                    }

                    break; 
                case PIT8253Mode::OneShot:
                    if (!current_counter.value)
                        current_counter.output = true;
                    else 
                        current_counter.value--;
                    break;
                case PIT8253Mode::RateGenerator:
                    current_counter.value--;
                    
                    if (current_counter.value <= 1)
                    {
                        current_counter.output = false; 
                        current_counter.value_on_reload = current_counter.value; 

                        switch (counter_id)
                        {
                            case 0x00:
                                Logging_LogChannel("PIC IRQ0 must be here, but isn't yet [PIT8253 Counter 0 Rate Generator zero value reached!]\n", LogChannel::Warning);    
                                break;
                            case 0x01:
                                Logging_LogChannel("DMA must be here, but isn't yet [PIT8253 Counter 1 Rate Generator zero value reached!]\n", LogChannel::Warning);    
                                break;
                        }
                    }      
                    else if (current_counter.value == current_counter.value_on_reload - 1)
                        current_counter.output = true; 

                    break;
                case PIT8253Mode::SquareWave:
                    // check if counter is odd
                    if (current_counter.value & 1)
                        current_counter.value -= (current_counter.output) ? 1 : 3;
                    else
                        current_counter.value -= 2; 

                    if (current_counter.value < 2)
                    {
                        current_counter.output = false;
                        current_counter.value = current_counter.value_on_reload;

                        switch (counter_id)
                        {
                            case 0x00:
                                Logging_LogChannel("PIC IRQ0 must be here, but isn't yet [PIT8253 Counter 0 Square Wave value reached!]\n", LogChannel::Warning);    
                                break;
                            case 0x01:
                                Logging_LogChannel("DMA2 must be here, but isn't yet [PIT8253 Counter 1 Square Wave value reached!]\n", LogChannel::Warning);    
                                break;
                        }
                    }

                    break;
                case PIT8253Mode::StrobeSoftware:
                case PIT8253Mode::StrobeHardware: //same but not retriggerable i think                
                    if (current_counter.output)
                    {
                        current_counter.value--;

                        if (!current_counter.value)
                            current_counter.output = false; 
                    }

                    break;
            }
        }
    }

    void PIT8253::Shutdown()
    {  
        IOx86_DeleteMapping(0x40, 0x43);
    }

    uint8_t PIT8253::PortRead(uint8_t port)
    {
        uint8_t ret = 0x00;
        uint8_t counter_index, access_mode, operating_mode; 
        uint16_t source_value;
        PIT8253Counter& current_counter = counters[port & 0x03]; 

        switch (port)
        {
            default:
                access_mode = (current_counter.control >> 4) & 0x03;
                operating_mode = (current_counter.control >> 1) & 0x07;

                source_value = (current_counter.latch_state) ? current_counter.value_on_latch : current_counter.value; 

                switch (access_mode)
                {
                    case 0x01:
                        ret = source_value & 0xFF;
                        break;
                    case 0x02:
                        ret = source_value >> 8;
                        break;
                    case 0x03:
                        if (!current_counter.read_2nd_byte)
                            ret = source_value & 0xFF;
                        else
                            ret = (source_value >> 8);
                        
                        current_counter.read_2nd_byte = !current_counter.read_2nd_byte;

                        if (current_counter.latch_state > 0)
                            current_counter.latch_state = 0; 
                }
                
                break; 
            case PIT8253Port::ControlWord:
                return 0x00; //cannot read this ort
        }

        Logging_LogChannel("Intel 8253/54 PIT port %04x read value=%04x", LogChannel::Debug, port, ret);

        return ret; 
    }

    void PIT8253::PortWrite(uint8_t port, uint8_t value)
    {
        uint8_t ret = 0x00;
        uint8_t counter_index, access_mode, operating_mode; 
        PIT8253Counter& current_counter = counters[port & 0x03]; 

        switch (port)
        {
            default:
                access_mode = (current_counter.control >> 4) & 0x03;
                operating_mode = (current_counter.control >> 1) & 0x07;

                //unlatch
                if (current_counter.value_on_latch)
                    current_counter.value_on_latch = 0; 
                
                // which byte are we writing
                switch (access_mode)
                {
                    case 0x01: // low byte only
                        current_counter.value_on_reload = (current_counter.value_on_reload & 0xFF);
                        break;
                    case 0x02: // high byte
                        current_counter.value_on_reload |= (value << 8);
                        break; 
                    case 0x03: // latch determines byte
                        if (current_counter.write_2nd_byte)
                            current_counter.value_on_reload |= (value << 8);
                        else
                            current_counter.value_on_reload = (current_counter.value_on_reload & 0xFF);
                        
                        current_counter.write_2nd_byte = !current_counter.write_2nd_byte;
                        
                        if (!operating_mode)
                            current_counter.running = current_counter.write_2nd_byte;

                        break;
                }

                if (operating_mode <= 2)
                {
                    current_counter.value = current_counter.value_on_reload;
                    current_counter.output = false;
                    current_counter.running = true;
                }
                

                break;
            case PIT8253Port::ControlWord:
                // determine which channel is being configured
                // set control
                counter_index = (value >> 6) & 0x03;

                //latch not needed for these
                current_counter = counters[counter_index];

                uint8_t access_mode = (value >> 4) & 0x03;

                // Latch only
                if (!access_mode)
                {
                    current_counter.latch_state = 1;
                    current_counter.value_on_latch = current_counter.value;
                }
                else   
                {
                    current_counter.control = value; 

                    if (current_counter.control & PIT8253Ctrl::BCD)
                        Logging_LogChannel("TODO: BCD mode not yet implemented!", LogChannel::Warning);    

                    uint8_t selected_counter = (current_counter.control >> 6) & 0x03;

                    // don't cast we need to xor it
                    operating_mode = (value >> 1) & 0x07;

                    if (operating_mode >= 6)
                    {
                        operating_mode ^= 4; 
                        current_counter.control = (current_counter.control & ~0xE) | operating_mode;
                    }

                    current_counter.write_2nd_byte = false; 

                }

                // why is the reference not working here
                counters[counter_index] = current_counter;

                break;
        }

        // why is the reference not working here
        Logging_LogChannel("Intel 8253/54 PIT port %04x write, value %04x", LogChannel::Debug, port, value);
    }
}