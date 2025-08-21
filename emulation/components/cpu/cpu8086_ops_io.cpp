// 
// VOLT
// Copyright © 2025 starfrost
//
// cpu8086_ops_io.cpp: I/O
//

#include <emulation/emulation.hpp>
#include <emulation/components/cpu/cpu8086.hpp>

namespace Volt
{
    void CPU8086::Op_In(uint8_t opcode)
    {
        bool port_16bit = (opcode & 0x08);
        bool value_16bit = (opcode & 0x01);

        if (!port_16bit)
        {
            uint8_t port = Prefetch_Pop8();

            if (value_16bit)
                ah = IOx86_Read(port + 1);

            al = IOx86_Read(port);
        }
        else
        {
            //write a 16bit value
            if (value_16bit)
                ah = IOx86_Read(dx + 1);
        
            al = IOx86_Read(dx);
        }
    }

    void CPU8086::Op_Out(uint8_t opcode)
    {
        bool port_16bit = (opcode & 0x08);
        bool value_16bit = (opcode & 0x01);

        if (!port_16bit)
        {            
            uint8_t port = Prefetch_Pop8();

            //write a 16bit value
            if (value_16bit)
                IOx86_Write(port + 1, ah);
        
            IOx86_Write(port, al);
        }
        else
        {
            //write a 16bit value
            if (value_16bit)
                IOx86_Write(dx + 1, ah);
        
            IOx86_Write(dx, al);
        }
    }
}