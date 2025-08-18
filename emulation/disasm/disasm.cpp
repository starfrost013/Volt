// 
// VOLT
// Copyright © 2025 starfrost
//
// disasm.cpp: Implements various disassembly functions (crappily)
//

#include <emulation/emulation.hpp>
#include <emulation/components/cpu/cpu8086.hpp>

namespace Volt
{
    // maximum size of the disassembly string buffer
    #define MAX_DISASM_BUF_SIZE         32
    
    //
    // 8086
    //

    // This function will never be called from outside of the main thread so it is ok to use a global
    // & has more efficient allocation than stackalloc & doesn't require putting a buffer in the cpu
    char disasm_buf_8086[MAX_DISASM_BUF_SIZE] = {0};
    char disasm_buf_scratch[MAX_DISASM_BUF_SIZE] = {0};     // scratch buffer used to build current operand before strncat'ing into disasm_buf_8086


    #define CPU8086_DISASM_SHORT_RELATIVE_START 0x60
    #define CPU8086_DISASM_SHORT_RELATIVE_END   0x7F
    #define CPU8086_DISASM_GRP1_START           0x80
    #define CPU8086_DISASM_GRP1_END             0x83
    #define CPU8086_DISASM_MOV_RELATIVE_START   0xB0
    #define CPU8086_DISASM_MOV_RELATIVE_END     0xBF
    #define CPU8086_DISASM_GRP2_START           0xD0
    #define CPU8086_DISASM_GRP2_END             0xD3
    #define CPU8086_DISASM_GRP3_8               0xF7
    #define CPU8086_DISASM_GRP3_16              0xF8
    #define CPU8086_DISASM_GRP4                 0xFE
    #define CPU8086_DISASM_GRP5                 0xFF

    // Should we encode the "type" in the instruction?
    #define CPU8086_DISASM_FAR_CALL             0xEA
    #define CPU8086_DISASM_FAR_JUMP             0x9A

    void CPU8086::Disasm_Parse(uint8_t opcode)
    {
        // clear buf
        memset(disasm_buf_8086, 0x00, MAX_DISASM_BUF_SIZE); // faster than strlen?
        memset(disasm_buf_scratch, 0x00, MAX_DISASM_BUF_SIZE); //faster than strlen?

        // set up the buffer

        strncat(disasm_buf_8086, opcode_table_disasm[opcode], MAX_DISASM_BUF_SIZE - 1);

        // First check 1BL instructions and return immediately
        if (instruction_table[opcode].size == 1)
            return;

        // linear_pc points to the modr/m byte

        if (opcode == CPU8086_DISASM_FAR_JUMP
        || opcode == CPU8086_DISASM_FAR_CALL)
        {
            snprintf(disasm_buf_scratch, MAX_DISASM_BUF_SIZE, " %04X:%04X", address_space->access_word(linear_pc() + 2), address_space->access_word(linear_pc()));
            strncat(disasm_buf_8086, disasm_buf_scratch, MAX_DISASM_BUF_SIZE - 1);
            return;
        }

        // mov r8,immed8 / mov r16,immed16
        if (opcode >= CPU8086_DISASM_MOV_RELATIVE_START
        && opcode <= CPU8086_DISASM_MOV_RELATIVE_END)
        {
            // This works because we only care about 40-4f
            if (opcode & 0x08)
            {
                snprintf(disasm_buf_scratch, MAX_DISASM_BUF_SIZE, " %s, %04X", register_table16_disasm[opcode % 0x08], address_space->access_word(linear_pc()));
                strncat(disasm_buf_8086, disasm_buf_scratch, MAX_DISASM_BUF_SIZE - 1);
            }
            else
            {
                snprintf(disasm_buf_scratch, MAX_DISASM_BUF_SIZE, " %s, %02X", register_table8_disasm[opcode % 0x08], address_space->access_byte[linear_pc()]);
                strncat(disasm_buf_8086, disasm_buf_scratch, MAX_DISASM_BUF_SIZE - 1);
            }

            return;
        }

        // short relative jumps
        if (opcode >= CPU8086_DISASM_SHORT_RELATIVE_START
        && opcode <= CPU8086_DISASM_SHORT_RELATIVE_END)
        {   
            uint8_t offset = address_space->access_word(linear_pc());
            //put a + in. - will come from the number
            if (offset & 0x80)
                snprintf(disasm_buf_scratch, MAX_DISASM_BUF_SIZE, " -%02X", (int8_t)offset);
            else
                snprintf(disasm_buf_scratch, MAX_DISASM_BUF_SIZE, " +%02X", (int8_t)offset);

            strncat(disasm_buf_8086, disasm_buf_scratch, MAX_DISASM_BUF_SIZE - 1);
            return;
        }  

        // tell modified modrm parser to ignore reg and instead print op
        bool ignore_reg = false;
        bool w = (opcode & 0x01);

        CPU8086::CPU8086InstructionModRM modrm_decode = {0}; 

        modrm_decode.modrm = address_space->access_byte[linear_pc()];
        
        // check for group instructions
        if ((opcode >= CPU8086_DISASM_GRP1_START && opcode <= CPU8086_DISASM_GRP1_END)
            || (opcode >= CPU8086_DISASM_GRP2_START && opcode <= CPU8086_DISASM_GRP2_END)
            || opcode == CPU8086_DISASM_GRP3_8
            || opcode == CPU8086_DISASM_GRP3_16
            || opcode == CPU8086_DISASM_GRP4
            || opcode == CPU8086_DISASM_GRP5)
            {
                ignore_reg = true;
            }

        // print the group instructions
        if (opcode >= CPU8086_DISASM_GRP1_START && opcode <= CPU8086_DISASM_GRP1_END)
            strncat(disasm_buf_8086, grp1_table_disasm[modrm_decode.reg], MAX_DISASM_BUF_SIZE - 1);

        if (opcode >= CPU8086_DISASM_GRP2_START && opcode <= CPU8086_DISASM_GRP2_END)
        {
            if (opcode & 0x02)
            {
                snprintf(disasm_buf_scratch, MAX_DISASM_BUF_SIZE, "%s %s, CL", 
                    grp2_table_disasm[modrm_decode.reg], (opcode & 0x01) ? register_table16_disasm[modrm_decode.rm] : register_table8_disasm[modrm_decode.rm]);
            }
            else
            {
                snprintf(disasm_buf_scratch, MAX_DISASM_BUF_SIZE, "%s %s, 1", 
                    grp2_table_disasm[modrm_decode.reg], (opcode & 0x01) ? register_table16_disasm[modrm_decode.rm] : register_table8_disasm[modrm_decode.rm]);
            }

            strncat(disasm_buf_8086, disasm_buf_scratch, MAX_DISASM_BUF_SIZE - 1);
            return;
        }
        
        if (opcode == CPU8086_DISASM_GRP3_8 || opcode == CPU8086_DISASM_GRP3_16)
            strncat(disasm_buf_8086, grp3_table_disasm[modrm_decode.reg], MAX_DISASM_BUF_SIZE - 1);
        
        if (opcode == CPU8086_DISASM_GRP4)
            strncat(disasm_buf_8086, grp4_table_disasm[modrm_decode.reg], MAX_DISASM_BUF_SIZE - 1);

        if (opcode == CPU8086_DISASM_GRP5)
            strncat(disasm_buf_8086, grp5_table_disasm[modrm_decode.reg], MAX_DISASM_BUF_SIZE - 1);
            

        // handle reg part (ignored in certain cases)
        // length can be handled by the opcode function -- it's part of the opcode anyway


        // default = DS for this part.
        uint16_t reg = ds;

        switch (modrm_decode.mod)
        {
            default:
                // Disp16
                if (modrm_decode.mod == 0x00 
                    && modrm_decode.rm == 6)
                {
                    snprintf(disasm_buf_scratch, MAX_DISASM_BUF_SIZE, "%04X", address_space->access_word(linear_pc() + 1));
                    strncat(disasm_buf_8086, disasm_buf_scratch, MAX_DISASM_BUF_SIZE - 1);
                    return;
                }
                 // now decode the RM
                
                switch (seg_override)
                {
                    case CPU8086::seg_override_es:
                        reg = es;
                        strncat(disasm_buf_8086, "ES:", MAX_DISASM_BUF_SIZE - 1);
                        break;
                    case CPU8086::seg_override_ss:
                        reg = ss;
                        strncat(disasm_buf_8086, "SS:", MAX_DISASM_BUF_SIZE - 1);
                        break; 
                    case CPU8086::seg_override_cs:
                        reg = cs;
                        strncat(disasm_buf_8086, "CS:", MAX_DISASM_BUF_SIZE - 1);
                        break;
                }
                
                if (reg == ds)
                    strncat(disasm_buf_8086, "DS:", MAX_DISASM_BUF_SIZE - 1);

                // >> 1 in order to fix the index
                strncat(disasm_buf_8086, rm_table_disasm[modrm_decode.rm], MAX_DISASM_BUF_SIZE - 1);

                strncat(disasm_buf_8086, "]", MAX_DISASM_BUF_SIZE - 1);

                if (modrm_decode.mod == 0x01) // +disp8
                {
                    snprintf(disasm_buf_scratch, MAX_DISASM_BUF_SIZE, ", %02X", address_space->access_byte[(linear_pc() + 1)]);
                    strncat(disasm_buf_8086, disasm_buf_scratch, MAX_DISASM_BUF_SIZE - 1);
                }
                else if (modrm_decode.mod == 0x02) // +disp16
                {
                    //>>1 due to values referring to 16 bit
                    snprintf(disasm_buf_scratch, MAX_DISASM_BUF_SIZE, ", %04X", address_space->access_word(linear_pc() + 1));
                    strncat(disasm_buf_8086, disasm_buf_scratch, MAX_DISASM_BUF_SIZE - 1);
                }

                break;        
            case 0x03:  // 11
                // This is a bit stupid. We cast it to 8 bit for 8 bit opcodes.
                if (w)
                    snprintf(disasm_buf_scratch, MAX_DISASM_BUF_SIZE, "%s, ", disasm_buf_8086, register_table16_disasm[modrm_decode.reg]);
                else
                    snprintf(disasm_buf_scratch, MAX_DISASM_BUF_SIZE, "%s, ", disasm_buf_8086, register_table8_disasm[modrm_decode.reg]);

                strncat(disasm_buf_8086, disasm_buf_scratch, MAX_DISASM_BUF_SIZE - 1);

                break;      
        }

        if (!ignore_reg && !((opcode >> 1) & 0x01))
        {
            if (w)
                snprintf(disasm_buf_scratch, MAX_DISASM_BUF_SIZE, ", %s", disasm_buf_8086, register_table16_disasm[modrm_decode.reg]);
            else
                snprintf(disasm_buf_scratch, MAX_DISASM_BUF_SIZE, ", %s", disasm_buf_8086, register_table8_disasm[modrm_decode.reg]);

            strncat(disasm_buf_8086, disasm_buf_scratch, MAX_DISASM_BUF_SIZE - 1);
        }

    }

    // Disassemble for the 8086
    void CPU8086::Disasm(uint8_t opcode)
    {
        Disasm_Parse(opcode);
        Logging_LogChannel(disasm_buf_8086, LogChannel::Debug);
    }

}