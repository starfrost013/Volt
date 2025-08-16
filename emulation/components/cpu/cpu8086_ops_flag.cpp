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

    void CPU8086::Op_Lahf()
    {
        Logging_LogChannel("LAHF", LogChannel::Debug);

        // the order of the flags is the same as 
        ah = (flags & 0xFF);

        //ensure invalid bits are correct
        ah &= ~CPU8086Flags::Reserved5;
        ah &= ~CPU8086Flags::Reserved3;
        ah |= CPU8086Flags::Reserved1;
    }

    void CPU8086::Op_Sahf()
    {
        Logging_LogChannel("SAHF", LogChannel::Debug);

        // preserve bits 15-8 of flags
        flags |= (ah & 0xFF);
    }
}