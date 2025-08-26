// 
// VOLT
// Copyright © 2025 starfrost
//
// cpu8086_ops_math.cpp: 8086 math ops
//

#include <emulation/emulation.hpp>
#include <emulation/components/cpu/cpu8086.hpp>

namespace Volt
{
    void CPU8086::Op_IncReg(uint8_t opcode)
    {
        uint16_t result = ++(*register_table16[opcode & 0x07]); 

        SetPZSFlags16(result);
        // Investigate functions to set these. A lot of them, however, are kind of one liner

        if (result & 0x0F == 0x00) 
            flags |= CPU8086Flags::AuxCarry;

        SetOF16_Add(result, result - 1, 1);
    }

    void CPU8086::Op_DecReg(uint8_t opcode)
    {
        uint16_t result = --(*register_table16[opcode & 0x07]); 
        
        SetPZSFlags16(result);

        if (result & 0x0F == 0x0F) 
            flags |= CPU8086Flags::AuxCarry;

        SetOF16_Sub(result, result - 1, 1);
    }

    //
    // BASIC MATH: ENTRY POINT
    //

    void CPU8086::Op_AddModRm(uint8_t opcode)
    {
        CPU8086InstructionModRM modrm = Decode_ModRM(opcode);

        bool w = opcode & 0x01;
        bool reverse = opcode & 0x02;

        if (!reverse)
            (w) ? Op_AddInternal16to16(modrm.ea_ptr, modrm.reg_ptr16) : Op_AddInternal8to8((uint8_t*)modrm.ea_ptr, modrm.reg_ptr8);
        else
            (w) ? Op_AddInternal16to16(modrm.reg_ptr16, modrm.ea_ptr) : Op_AddInternal8to8(modrm.reg_ptr8, (uint8_t*)modrm.ea_ptr);
    }

    void CPU8086::Op_AddImmed(uint8_t opcode)
    {
        bool is_16 = (opcode & 0x01);

        uint8_t immed8 = 0x00;
        uint16_t immed16 = 0x00;

        if (!is_16)
            immed8 = Prefetch_Pop8();
        else
            immed16 = Prefetch_Pop16();

        if (!is_16)  
            Op_AddInternal8to8(&al, &immed8);
        else
            Op_AddInternal16to16(&ax, &immed16);

    }

    void CPU8086::Op_OrModRm(uint8_t opcode)
    {
        CPU8086InstructionModRM modrm = Decode_ModRM(opcode);

        bool w = opcode & 0x01;
        bool reverse = opcode & 0x02;

        if (!reverse)
            (w) ? Op_OrInternal16to16(modrm.ea_ptr, modrm.reg_ptr16) : Op_OrInternal8to8((uint8_t*)modrm.ea_ptr, modrm.reg_ptr8);
        else
            (w) ? Op_OrInternal16to16(modrm.reg_ptr16, modrm.ea_ptr) : Op_OrInternal8to8(modrm.reg_ptr8, (uint8_t*)modrm.ea_ptr);
    }

    void CPU8086::Op_OrImmed(uint8_t opcode)
    {
        bool is_16 = (opcode & 0x01);

        uint8_t immed8 = 0x00;
        uint16_t immed16 = 0x00;

        if (!is_16)
            immed8 = Prefetch_Pop8();
        else
            immed16 = Prefetch_Pop16();

        if (!is_16)  
            Op_OrInternal8to8(&al, &immed8);
        else
            Op_OrInternal16to16(&ax, &immed16);
    }

    void CPU8086::Op_AdcModRm(uint8_t opcode)
    {
        CPU8086InstructionModRM modrm = Decode_ModRM(opcode);

        bool w = opcode & 0x01;
        bool reverse = opcode & 0x02;

        if (!reverse)
            (w) ? Op_AdcInternal16to16(modrm.ea_ptr, modrm.reg_ptr16) : Op_AdcInternal8to8((uint8_t*)modrm.ea_ptr, modrm.reg_ptr8);
        else
            (w) ? Op_AdcInternal16to16(modrm.reg_ptr16, modrm.ea_ptr) : Op_AdcInternal8to8(modrm.reg_ptr8, (uint8_t*)modrm.ea_ptr);
    }

    void CPU8086::Op_AdcImmed(uint8_t opcode)
    {
        bool is_16 = (opcode & 0x01);

        uint8_t immed8 = 0x00;
        uint16_t immed16 = 0x00;

        if (!is_16)
            immed8 = Prefetch_Pop8();
        else
            immed16 = Prefetch_Pop16();

        if (!is_16)  
            Op_AdcInternal8to8(&al, &immed8);
        else
            Op_AdcInternal16to16(&ax, &immed16);
    }

    void CPU8086::Op_SbbModRm(uint8_t opcode)
    {
        CPU8086InstructionModRM modrm = Decode_ModRM(opcode);

        bool w = opcode & 0x01;
        bool reverse = opcode & 0x02;

        if (!reverse)
            (w) ? Op_SbbInternal16to16(modrm.ea_ptr, modrm.reg_ptr16) : Op_SbbInternal8to8((uint8_t*)modrm.ea_ptr, modrm.reg_ptr8);
        else
            (w) ? Op_SbbInternal16to16(modrm.reg_ptr16, modrm.ea_ptr) : Op_SbbInternal8to8(modrm.reg_ptr8, (uint8_t*)modrm.ea_ptr);
    }

    void CPU8086::Op_SbbImmed(uint8_t opcode)
    {
        bool is_16 = (opcode & 0x01);

        uint8_t immed8 = 0x00;
        uint16_t immed16 = 0x00;

        if (!is_16)
            immed8 = Prefetch_Pop8();
        else
            immed16 = Prefetch_Pop16();

        if (!is_16)  
            Op_SbbInternal8to8(&al, &immed8);
        else
            Op_SbbInternal16to16(&ax, &immed16);
    }

    void CPU8086::Op_AndModRm(uint8_t opcode)
    {
        CPU8086InstructionModRM modrm = Decode_ModRM(opcode);

        bool w = opcode & 0x01;
        bool reverse = opcode & 0x02;

        if (!reverse)
            (w) ? Op_AndInternal16to16(modrm.ea_ptr, modrm.reg_ptr16) : Op_AndInternal8to8((uint8_t*)modrm.ea_ptr, modrm.reg_ptr8);
        else
            (w) ? Op_AndInternal16to16(modrm.reg_ptr16, modrm.ea_ptr) : Op_AndInternal8to8(modrm.reg_ptr8, (uint8_t*)modrm.ea_ptr);
    }

    void CPU8086::Op_AndImmed(uint8_t opcode)
    {
        bool is_16 = (opcode & 0x01);

        uint8_t immed8 = 0x00;
        uint16_t immed16 = 0x00;

        if (!is_16)
            immed8 = Prefetch_Pop8();
        else
            immed16 = Prefetch_Pop16();

        if (!is_16)  
            Op_AndInternal8to8(&al, &immed8);
        else
            Op_AndInternal16to16(&ax, &immed16);
    }

    void CPU8086::Op_SubModRm(uint8_t opcode)
    {
        CPU8086InstructionModRM modrm = Decode_ModRM(opcode);

        bool w = opcode & 0x01;
        bool reverse = opcode & 0x02;

        if (!reverse)
            (w) ? Op_SubInternal16to16(modrm.ea_ptr, modrm.reg_ptr16) : Op_SubInternal8to8((uint8_t*)modrm.ea_ptr, modrm.reg_ptr8);
        else
            (w) ? Op_SubInternal16to16(modrm.reg_ptr16, modrm.ea_ptr) : Op_SubInternal8to8(modrm.reg_ptr8, (uint8_t*)modrm.ea_ptr);
    }

    void CPU8086::Op_SubImmed(uint8_t opcode)
    {
        bool is_16 = (opcode & 0x01);

        uint8_t immed8 = 0x00;
        uint16_t immed16 = 0x00;

        if (!is_16)
            immed8 = Prefetch_Pop8();
        else
            immed16 = Prefetch_Pop16();

        if (!is_16)  
            Op_SubInternal8to8(&al, &immed8);
        else
            Op_SubInternal16to16(&ax, &immed16);
    }

    void CPU8086::Op_XorModRm(uint8_t opcode)
    {
        CPU8086InstructionModRM modrm = Decode_ModRM(opcode);

        bool w = opcode & 0x01;
        bool reverse = opcode & 0x02;

        if (!reverse)
            (w) ? Op_XorInternal16to16(modrm.ea_ptr, modrm.reg_ptr16) : Op_XorInternal8to8((uint8_t*)modrm.ea_ptr, modrm.reg_ptr8);
        else
            (w) ? Op_XorInternal16to16(modrm.reg_ptr16, modrm.ea_ptr) : Op_XorInternal8to8(modrm.reg_ptr8, (uint8_t*)modrm.ea_ptr);
    }

    void CPU8086::Op_XorImmed(uint8_t opcode)
    {
        bool is_16 = (opcode & 0x01);

        uint8_t immed8 = 0x00;
        uint16_t immed16 = 0x00;

        if (!is_16)
            immed8 = Prefetch_Pop8();
        else
            immed16 = Prefetch_Pop16();

        if (!is_16)  
            Op_XorInternal8to8(&al, &immed8);
        else
            Op_XorInternal16to16(&ax, &immed16);
    }

    void CPU8086::Op_CmpModRm(uint8_t opcode)
    {
        CPU8086InstructionModRM modrm = Decode_ModRM(opcode);

        bool w = opcode & 0x01;
        bool reverse = opcode & 0x02;

        if (!reverse)
            (w) ? Op_CmpInternal16to16(modrm.ea_ptr, modrm.reg_ptr16) : Op_CmpInternal8to8((uint8_t*)modrm.ea_ptr, modrm.reg_ptr8);
        else
            (w) ? Op_CmpInternal16to16(modrm.reg_ptr16, modrm.ea_ptr) : Op_CmpInternal8to8(modrm.reg_ptr8, (uint8_t*)modrm.ea_ptr);
    }

    void CPU8086::Op_CmpImmed(uint8_t opcode)
    {
        bool is_16 = (opcode & 0x01);

        uint8_t immed8 = 0x00;
        uint16_t immed16 = 0x00;

        if (!is_16)
            immed8 = Prefetch_Pop8();
        else
            immed16 = Prefetch_Pop16();

        if (!is_16)  
            Op_XorInternal8to8(&al, &immed8);
        else
            Op_XorInternal16to16(&ax, &immed16);
    }

    //
    // BASIC MATH: INTERNAL 
    //

    void CPU8086::Op_AddInternal8to8(uint8_t* dst_ptr, uint8_t* src_ptr)
    {
        uint8_t src = *src_ptr; 
        uint8_t old_dst = *dst_ptr; 
        *dst_ptr = old_dst + src;
        uint8_t new_dst = *dst_ptr;

        SetPZSFlags8(new_dst);
        SetOF8_Add(new_dst, old_dst, src);

        if ((src ^ old_dst ^ new_dst) & 0x10)
            flags |= CPU8086Flags::AuxCarry;
        
        if (new_dst < src)
            flags |= CPU8086Flags::Carry;
    }

    void CPU8086::Op_AddInternal8to16(uint16_t* dst_ptr, uint8_t* src_ptr)
    {
        uint8_t src = *src_ptr; 
        uint16_t old_dst = *dst_ptr; 
        *dst_ptr = old_dst + src;
        uint16_t new_dst = *dst_ptr;

        SetPZSFlags16(new_dst);
        SetOF16_Add(new_dst, old_dst, src);

        if ((src ^ old_dst ^ new_dst) & 0x10)
            flags |= CPU8086Flags::AuxCarry;
        
        if (new_dst < src)
            flags |= CPU8086Flags::Carry;
    }

    void CPU8086::Op_AddInternal16to16(uint16_t* dst_ptr, uint16_t* src_ptr)
    {
        uint16_t src = *src_ptr; 
        uint16_t old_dst = *dst_ptr; 
        *dst_ptr = old_dst + src;
        uint16_t new_dst = *dst_ptr;

        SetPZSFlags16(new_dst);
        SetOF16_Add(new_dst, old_dst, src);

        if ((src ^ old_dst ^ new_dst) & 0x10)
            flags |= CPU8086Flags::AuxCarry;
        
        if (new_dst < src)
            flags |= CPU8086Flags::Carry;
    }

    void CPU8086::Op_OrInternal8to8(uint8_t* dst_ptr, uint8_t* src_ptr)
    {
        uint8_t old_dst = *dst_ptr;
        *dst_ptr = old_dst | *src_ptr;

        SetPZSFlags8(*dst_ptr);
        // What does OR/XOR/AND do with auxcarry?
        flags &= ~(CPU8086Flags::Carry);
        flags &= ~(CPU8086Flags::Overflow);
    }

    void CPU8086::Op_OrInternal8to16(uint16_t* dst_ptr, uint8_t* src_ptr)
    {
        uint16_t old_dst = *dst_ptr;
        *dst_ptr = old_dst | *src_ptr;

        SetPZSFlags16(*dst_ptr);
        flags &= ~(CPU8086Flags::Carry);
        flags &= ~(CPU8086Flags::Overflow);
    }

    void CPU8086::Op_OrInternal16to16(uint16_t* dst_ptr, uint16_t* src_ptr)
    {
        uint16_t old_dst = *dst_ptr;
        *dst_ptr = old_dst | *src_ptr;

        SetPZSFlags16(*dst_ptr);
        flags &= ~(CPU8086Flags::Carry);
        flags &= ~(CPU8086Flags::Overflow);
    }

    void CPU8086::Op_AdcInternal8to8(uint8_t* dst_ptr, uint8_t* src_ptr)
    {
        uint8_t src = *src_ptr; 
        uint8_t old_dst = *dst_ptr; 
        *dst_ptr = old_dst + src + (flags & CPU8086Flags::Carry); //1 so no OR needed
        uint8_t new_dst = *dst_ptr;

        SetPZSFlags8(new_dst);
        SetOF8_Add(new_dst, old_dst, src);

        if ((src & 0x0F) + (old_dst & 0x0F) + (flags & CPU8086Flags::Carry) >= 0x10)
            flags |= CPU8086Flags::AuxCarry;

        if ((new_dst + (flags & CPU8086Flags::Carry) >> 8) > 0)
            flags |= CPU8086Flags::Carry;
    }

    void CPU8086::Op_AdcInternal8to16(uint16_t* dst_ptr, uint8_t* src_ptr)
    {
        uint8_t src = *src_ptr; 
        uint16_t old_dst = *dst_ptr; 
        *dst_ptr = old_dst + src + (flags & CPU8086Flags::Carry); //1 so no OR needed
        uint16_t new_dst = *dst_ptr;

        SetPZSFlags16(new_dst);
        SetOF16_Add(new_dst, old_dst, src);

        if ((src & 0x0F) + (old_dst & 0x0F) + (flags & CPU8086Flags::Carry) >= 0x10)
            flags |= CPU8086Flags::AuxCarry;

        if ((new_dst + (flags & CPU8086Flags::Carry) >> 8) > 0)
            flags |= CPU8086Flags::Carry;
    }

    void CPU8086::Op_AdcInternal16to16(uint16_t* dst_ptr, uint16_t* src_ptr)
    {
        uint16_t src = *src_ptr; 
        uint16_t old_dst = *dst_ptr; 
        *dst_ptr = old_dst + src + (flags & CPU8086Flags::Carry); //1 so no OR needed
        uint16_t new_dst = *dst_ptr;

        SetPZSFlags16(new_dst);
        SetOF16_Add(new_dst, old_dst, src);

        if ((src & 0x0F) + (old_dst & 0x0F) + (flags & CPU8086Flags::Carry) >= 0x10)
            flags |= CPU8086Flags::AuxCarry;

        if ((new_dst + (flags & CPU8086Flags::Carry) >> 8) > 0)
            flags |= CPU8086Flags::Carry;
    }

    void CPU8086::Op_SbbInternal8to8(uint8_t* dst_ptr, uint8_t* src_ptr)
    {
        uint8_t src = *src_ptr; 
        uint8_t old_dst = *dst_ptr; 
        *dst_ptr = old_dst - (src + (flags & CPU8086Flags::Carry)); // borrow: 1 so no OR needed
        uint8_t new_dst = *dst_ptr;

        SetPZSFlags8(new_dst);
        SetOF8_Add(new_dst, old_dst, src);

        // we need to redo the operation here, due to casting to signed
        if (((int8_t)old_dst & 0x0F) - (((int8_t)src & 0x0F) + (flags & CPU8086Flags::Carry)) < 0)
            flags |= CPU8086Flags::AuxCarry;

        if ((int8_t)old_dst - ((int8_t)src + (flags & CPU8086Flags::Carry) >> 8) < 0)
            flags |= CPU8086Flags::Carry;
    }

    void CPU8086::Op_SbbInternal8to16(uint16_t* dst_ptr, uint8_t* src_ptr)
    {
        uint8_t src = *src_ptr; 
        uint16_t old_dst = *dst_ptr; 
        *dst_ptr = old_dst - (src + (flags & CPU8086Flags::Carry)); // borrow: 1 so no OR needed
        uint16_t new_dst = *dst_ptr;

        SetPZSFlags16(new_dst);
        SetOF16_Add(new_dst, old_dst, src);

        // we need to redo the operation here, due to casting to signed
        if (((int8_t)old_dst & 0x0F) - (((int16_t)src & 0x0F) + (flags & CPU8086Flags::Carry)) < 0)
            flags |= CPU8086Flags::AuxCarry;

        if ((int8_t)old_dst - ((int16_t)src + (flags & CPU8086Flags::Carry) >> 8) < 0)
            flags |= CPU8086Flags::Carry;
    }

    void CPU8086::Op_SbbInternal16to16(uint16_t* dst_ptr, uint16_t* src_ptr)
    {
        uint16_t src = *src_ptr; 
        uint16_t old_dst = *dst_ptr; 
        *dst_ptr = old_dst - (src + (flags & CPU8086Flags::Carry)); // borrow: 1 so no OR needed
        uint16_t new_dst = *dst_ptr;

        SetPZSFlags16(new_dst);
        SetOF16_Add(new_dst, old_dst, src);

        // we need to redo the operation here, due to casting to signed
        if (((int16_t)old_dst & 0x0F) - (((int16_t)src & 0x0F) + (flags & CPU8086Flags::Carry)) < 0)
            flags |= CPU8086Flags::AuxCarry;

        if ((int16_t)old_dst - ((int16_t)src + (flags & CPU8086Flags::Carry) >> 8) < 0)
            flags |= CPU8086Flags::Carry;
    }

    void CPU8086::Op_AndInternal8to8(uint8_t* dst_ptr, uint8_t* src_ptr)
    {
        uint8_t old_dst = *dst_ptr;
        *dst_ptr = old_dst & *src_ptr;

        SetPZSFlags8(*dst_ptr);
        flags &= ~(CPU8086Flags::Carry);
        flags &= ~(CPU8086Flags::Overflow);
    }

    void CPU8086::Op_AndInternal8to16(uint16_t* dst_ptr, uint8_t* src_ptr)
    {
        uint16_t old_dst = *dst_ptr;
        *dst_ptr = old_dst & *src_ptr;

        SetPZSFlags16(*dst_ptr);
        flags &= ~(CPU8086Flags::Carry);
        flags &= ~(CPU8086Flags::Overflow);
    }

    void CPU8086::Op_AndInternal16to16(uint16_t* dst_ptr, uint16_t* src_ptr)
    {
        uint16_t old_dst = *dst_ptr;
        *dst_ptr = old_dst & *src_ptr;

        SetPZSFlags16(*dst_ptr);
        flags &= ~(CPU8086Flags::Carry);
        flags &= ~(CPU8086Flags::Overflow);
    }

    void CPU8086::Op_SubInternal8to8(uint8_t* dst_ptr, uint8_t* src_ptr)
    {
        uint8_t src = *src_ptr; 
        uint8_t old_dst = *dst_ptr; 
        *dst_ptr = old_dst - src;
        uint8_t new_dst = *dst_ptr;

        SetPZSFlags8(new_dst);
        SetOF8_Sub(new_dst, old_dst, src);

        if ((src ^ old_dst ^ new_dst) & 0x10)
            flags |= CPU8086Flags::AuxCarry;
        
       //check
        if (src > old_dst)
            flags |= CPU8086Flags::Carry;
    }

    void CPU8086::Op_SubInternal8to16(uint16_t* dst_ptr, uint8_t* src_ptr)
    {
        uint8_t src = *src_ptr; 
        uint16_t old_dst = *dst_ptr; 
        *dst_ptr = old_dst - src;
        uint16_t new_dst = *dst_ptr;

        SetPZSFlags16(new_dst);
        SetOF16_Sub(new_dst, old_dst, src);

        if ((src ^ old_dst ^ new_dst) & 0x10)
            flags |= CPU8086Flags::AuxCarry;
        
        //check
        if (src > old_dst)
            flags |= CPU8086Flags::Carry;
    }

    void CPU8086::Op_SubInternal16to16(uint16_t* dst_ptr, uint16_t* src_ptr)
    {
        uint16_t src = *src_ptr; 
        uint16_t old_dst = *dst_ptr; 
        *dst_ptr = old_dst - src;
        uint16_t new_dst = *dst_ptr;

        SetPZSFlags16(new_dst);
        SetOF16_Sub(new_dst, old_dst, src);

        if ((src ^ old_dst ^ new_dst) & 0x10)
            flags |= CPU8086Flags::AuxCarry;
        
        //check
        if (src > old_dst)
            flags |= CPU8086Flags::Carry;
    }

    void CPU8086::Op_XorInternal8to8(uint8_t* dst_ptr, uint8_t* src_ptr)
    {
        uint8_t old_dst = *dst_ptr;
        *dst_ptr = old_dst ^ *src_ptr;

        SetPZSFlags8(*dst_ptr);
        flags &= ~(CPU8086Flags::Carry);
        flags &= ~(CPU8086Flags::Overflow);
    }

    void CPU8086::Op_XorInternal8to16(uint16_t* dst_ptr, uint8_t* src_ptr)
    {
        uint16_t old_dst = *dst_ptr;
        *dst_ptr = old_dst ^ *src_ptr;

        SetPZSFlags16(*dst_ptr);
        flags &= ~(CPU8086Flags::Carry);
        flags &= ~(CPU8086Flags::Overflow);
    }

    void CPU8086::Op_XorInternal16to16(uint16_t* dst_ptr, uint16_t* src_ptr)
    {
        uint16_t old_dst = *dst_ptr;
        *dst_ptr = old_dst ^ *src_ptr;

        SetPZSFlags16(*dst_ptr);
        flags &= ~(CPU8086Flags::Carry);
        flags &= ~(CPU8086Flags::Overflow);
    }

    //
    // CMP is sub but restore old_dst
    //

    void CPU8086::Op_CmpInternal8to8(uint8_t* dst_ptr, uint8_t* src_ptr)
    {
        uint8_t old = *dst_ptr;
        Op_SubInternal8to8(dst_ptr, src_ptr);
        *dst_ptr = old;
    }

    void CPU8086::Op_CmpInternal8to16(uint16_t* dst_ptr, uint8_t* src_ptr)
    {
        uint16_t old = *dst_ptr;
        Op_SubInternal8to16(dst_ptr, src_ptr);
        *dst_ptr = old;
    }

    void CPU8086::Op_CmpInternal16to16(uint16_t* dst_ptr, uint16_t* src_ptr)
    {
        uint16_t old = *dst_ptr;
        Op_SubInternal16to16(dst_ptr, src_ptr);
        *dst_ptr = old;
    }

    // Test is the same as and, but the result is discarded.
    void CPU8086::Op_TestModRm(uint8_t opcode)
    {
        bool w = (opcode & 0x01);
        CPU8086InstructionModRM modrm = Decode_ModRM(opcode); 

        // Eb, Gb
        if (!w)
        {
            uint8_t old_ea_ptr = *(uint8_t*)modrm.ea_ptr;

            Op_AndInternal8to8((uint8_t*)modrm.ea_ptr, modrm.reg_ptr8);
            *(uint8_t*)modrm.ea_ptr = old_ea_ptr;
        }
        // Ev, Gv
        else
        {
            uint16_t old_ea_ptr = *modrm.ea_ptr;

            Op_AndInternal16to16(modrm.ea_ptr, modrm.reg_ptr16);
            *modrm.ea_ptr = old_ea_ptr;
        }
    }

    // Test is the same as and, but the result is discarded.
    void CPU8086::Op_TestImmed(uint8_t opcode)
    {
        bool w = (opcode & 0x01);

        if (!w)
        {
            uint8_t old_al = al;
            uint8_t immed8 = Prefetch_Pop8();

            Op_AndInternal8to8(&al, &immed8);
            al = old_al;
        }
        else
        {
            uint16_t old_ax = ax;
            uint16_t immed16 = Prefetch_Pop16();

            Op_AndInternal16to16(&ax, &immed16);
            ax = old_ax;
        }
    }
    
}