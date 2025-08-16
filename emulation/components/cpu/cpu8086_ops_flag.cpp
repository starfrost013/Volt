// 
// VOLT
// Copyright © 2025 starfrost
//
// cpu8086_ops_flag.cpp: 8086 flag ops
//

#include <emulation/emulation.hpp>
#include <emulation/components/cpu/cpu8086.hpp>

namespace Volt
{
    void CPU8086::Op_Cli()
    {
        Logging_LogChannel("CLI", LogChannel::Debug);
        
        flags &= ~CPU8086Flags::IntrEnable;
    }
    
    void CPU8086::Op_Sti()
    {
        Logging_LogChannel("STI", LogChannel::Debug);

        flags |= CPU8086Flags::IntrEnable;
    }

    void CPU8086::Op_Clc()
    {
        Logging_LogChannel("CLC", LogChannel::Debug);
        
        flags &= ~CPU8086Flags::Carry;
    }
    
    void CPU8086::Op_Stc()
    {
        Logging_LogChannel("STC", LogChannel::Debug);

        flags |= CPU8086Flags::Carry;
    }

    void CPU8086::Op_Cmc()
    {
        Logging_LogChannel("CMC", LogChannel::Debug);

        if (flags & CPU8086Flags::Carry)
            flags &= ~CPU8086Flags::Carry;
        else
            flags |= CPU8086Flags::Carry;
    }

    void CPU8086::Op_Cld()
    {
        Logging_LogChannel("CLD", LogChannel::Debug);
        
        flags &= ~CPU8086Flags::Direction;
    }
    
    void CPU8086::Op_Std()
    {
        Logging_LogChannel("STD", LogChannel::Debug);

        flags |= CPU8086Flags::Direction;
    }
}