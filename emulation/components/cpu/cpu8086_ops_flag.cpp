// 
// VOLT
// Copyright © 2025 starfrost
//
// cpu8086_ops_flag.cpp: 8086 flag ops
//

#include "cpu8086.hpp"
#include <emulation/emulation.hpp>
#include <emulation/components/cpu/cpu8086.hpp>

namespace Volt
{
    void CPU8086::Op_Cli(uint8_t opcode)
    {
        //Logging_LogChannel("CLI", LogChannel::Debug);
        
        flags &= ~CPU8086Flags::IntrEnable;
    }
    
    void CPU8086::Op_Sti(uint8_t opcode)
    {
        //Logging_LogChannel("STI", LogChannel::Debug);

        flags |= CPU8086Flags::IntrEnable;
    }

    void CPU8086::Op_Clc(uint8_t opcode)
    {
        //Logging_LogChannel("CLC", LogChannel::Debug);
        
        flags &= ~CPU8086Flags::Carry;
    }
    
    void CPU8086::Op_Stc(uint8_t opcode)
    {
        //Logging_LogChannel("STC", LogChannel::Debug);

        flags |= CPU8086Flags::Carry;
    }

    void CPU8086::Op_Cmc(uint8_t opcode)
    {
        //Logging_LogChannel("CMC", LogChannel::Debug);

        if (flags & CPU8086Flags::Carry)
            flags &= ~CPU8086Flags::Carry;
        else
            flags |= CPU8086Flags::Carry;
    }

    void CPU8086::Op_Cld(uint8_t opcode)
    {
        //Logging_LogChannel("CLD", LogChannel::Debug);
        
        flags &= ~CPU8086Flags::Direction;
    }
    
    void CPU8086::Op_Std(uint8_t opcode)
    {
        //Logging_LogChannel("STD", LogChannel::Debug);

        flags |= CPU8086Flags::Direction;
    }

    void CPU8086::Op_Lahf(uint8_t opcode)
    {
        //Logging_LogChannel("LAHF", LogChannel::Debug);

        // the order of the flags is the same as 
        ah = (flags & 0xFF);

        //ensure invalid bits are correct
        ah &= ~CPU8086Flags::Reserved5;
        ah &= ~CPU8086Flags::Reserved3;
        ah |= CPU8086Flags::Reserved1;
    }

    void CPU8086::Op_Sahf(uint8_t opcode)
    {
        //Logging_LogChannel("SAHF", LogChannel::Debug);

        // preserve bits 15-8 of flags
        uint16_t old_high_flags = (flags >> 8) << 8; 

        flags = (old_high_flags) | ah;
    }

    // These don't compile marked as inline wtf
    void CPU8086::SetPZSFlags8(uint8_t result)
    {
        (result) ? flags &= ~CPU8086Flags::Zero : flags |= CPU8086Flags::Zero;
        (byte_parity_table[result & 0xFF]) ? flags |= CPU8086Flags::Parity : flags &= ~CPU8086Flags::Parity; 
        (result & 0x80) ? flags |= CPU8086Flags::Sign : flags &= ~CPU8086Flags::Sign;
    }

    // These don't compile marked as inline wtf
    void CPU8086::SetPZSFlags16(uint16_t result)
    {
        (result) ? flags &= ~CPU8086Flags::Zero : flags |= CPU8086Flags::Zero;
        (byte_parity_table[result & 0xFF]) ? flags |= CPU8086Flags::Parity : flags &= ~CPU8086Flags::Parity; 
        (result & 0x8000) ? flags |= CPU8086Flags::Sign : flags &= ~CPU8086Flags::Sign;
    }

    // Set overflow flag for 8 bit add operations.
    void CPU8086::SetOF8_Add(uint8_t result, uint8_t old_result, uint8_t operand)
    {
        if (((result ^ old_result) & (result ^ operand)) & 0x80)
            flags |= CPU8086Flags::Overflow;
        else
            flags &= ~CPU8086Flags::Overflow;
    }
    
    // Set overflow flag for 8 bit sub ops.
    void CPU8086::SetOF8_Sub(uint8_t result, uint8_t old_result, uint8_t operand)
    {
        if (((operand ^ old_result) & (operand ^ result)) & 0x80)
            flags |= CPU8086Flags::Overflow;
        else
            flags &= ~CPU8086Flags::Overflow;
    }
    
    void CPU8086::SetOF16_Add(uint16_t result, uint16_t old_result, uint16_t operand)
    {
        if (((result ^ old_result) & (result ^ operand)) & 0x8000)
            flags |= CPU8086Flags::Overflow;
        else
            flags &= ~CPU8086Flags::Overflow;
    }

    void CPU8086::SetOF16_Sub(uint16_t result, uint16_t old_result, uint16_t operand)
    {
        if (((operand ^ old_result) & (operand ^ result)) & 0x8000)
            flags |= CPU8086Flags::Overflow;
        else
            flags &= ~CPU8086Flags::Overflow;
    }
}