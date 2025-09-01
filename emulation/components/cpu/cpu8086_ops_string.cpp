// 
// VOLT
// Copyright © 2025 starfrost
//
// cpu8086_ops_string.cpp: 8086 move ops
//

#include "cpu8086.hpp"
#include <emulation/emulation.hpp>
#include <emulation/components/cpu/cpu8086.hpp>

namespace Volt
{
    void CPU8086::Op_RepPrefix(uint8_t opcode)
    {
        (opcode & 0x01) ? rep_type = CPU8086RepType::Repz : rep_type = CPU8086RepType::Rep;
    }

    // Internal function to update the string location for 8 bit string ops
    void CPU8086::Op_UpdateStringLocation8()
    {
        si += ((flags & CPU8086Flags::Direction) ? -1 : 1) & CPU8086_SEGMENT_SIZE;
        di += ((flags & CPU8086Flags::Direction) ? -1 : 1) & CPU8086_SEGMENT_SIZE;
    }

    // Internal function to update the string location for 16 bit string ops
    void CPU8086::Op_UpdateStringLocation16()
    {
        si += ((flags & CPU8086Flags::Direction) ? -2 : 2) & CPU8086_SEGMENT_SIZE;
        di += ((flags & CPU8086Flags::Direction) ? -2 : 2) & CPU8086_SEGMENT_SIZE;
    }

    void CPU8086::Op_Movs(uint8_t opcode)
    {
        bool w = (opcode & 0x01);

        uint32_t src_location = linear_ds_si();
        uint32_t dst_location = linear_es_di();

        // override supported for ds but not es
        if (!seg_override_reg_ptr)
            src_location = ((*seg_override_reg_ptr << 4) + si);
        
        if (w) // movsw
        {
            address_space->write_word(dst_location, address_space->read_word(src_location));
            Op_UpdateStringLocation16();
        }
        else // movsb
        {
            address_space->write_byte(dst_location, address_space->read_byte(src_location));
            Op_UpdateStringLocation8();
        }
    }

    // Compare two strings
    void CPU8086::Op_Cmps(uint8_t opcode)
    {
        bool w = (opcode & 0x01);

        uint32_t src_location = linear_ds_si();
        uint32_t dst_location = linear_es_di();

        // override supported for ds but not es
        if (!seg_override_reg_ptr)
            src_location = ((*seg_override_reg_ptr << 4) + si);

        if (w)
        {
            uint16_t real_start16 = address_space->read_word(src_location);
            uint16_t real_end16 = address_space->read_word(dst_location);

            Op_CmpInternal16to16(&real_end16, &real_start16);
            Op_UpdateStringLocation16();
        }
        else
        {
            uint8_t real_start8 = address_space->read_byte(src_location);
            uint8_t real_end8 = address_space->read_byte(dst_location);

            Op_CmpInternal8to8(&real_end8, &real_start8);
            Op_UpdateStringLocation8();
        }
    }

    void CPU8086::Op_Stos(uint8_t opcode)
    {
        bool w = (opcode & 0x01);
        uint32_t dst_location = linear_es_di();

        if (w)
        {
            address_space->write_word(dst_location, ax);
            Op_UpdateStringLocation16();
        }
        else
        {
            address_space->write_byte(dst_location, al); 
            Op_UpdateStringLocation8();
        }
    }

    // Load a string into AL/AX
    void CPU8086::Op_Lods(uint8_t opcode)
    {
        bool w = (opcode & 0x01);
        uint32_t dst_location = linear_es_di();

        // only SI modified by this one

        if (w)
        {
            ax = address_space->read_word(dst_location);
            si += (flags & CPU8086Flags::Direction) ? -2 : 2;
            si %= CPU8086_SEGMENT_SIZE;
        }
        else
        {
            al = address_space->read_byte(dst_location);
            si += (flags & CPU8086Flags::Direction) ? -1 : 1;
            si %= CPU8086_SEGMENT_SIZE;
        }
    }

    void CPU8086::Op_Scas(uint8_t opcode)
    {
        bool w = (opcode & 0x01);

        uint32_t dst_location = linear_es_di();

        if (w)
        {
            uint16_t real_end16 = address_space->read_word(dst_location);

            Op_CmpInternal16to16(&ax, &real_end16);
            Op_UpdateStringLocation16();
        }
        else
        {
            uint8_t real_end8 = address_space->read_byte(dst_location);

            Op_CmpInternal8to8(&al, &real_end8);
            Op_UpdateStringLocation8();
        }
    }
}