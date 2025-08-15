// 
// VOLT
// Copyright © 2025 starfrost
//
// CPU8086.hpp: Generic 808x-class CPU (8086, 8088, V30)
//
#pragma once
#include <emulation/emulation.hpp>

// BITFIELDS ARE IN REVERSE TO LOGICAL ORDER!

namespace Volt
{
    #define CPU8086_ADDR_SPACE_SIZE     1000000
    #define CPU8086_NUM_OPCODES         256
    #define CPU8086_NUM_REGISTERS       8

    class CPU8086 : public Component
    {
        public:
            const char* name;

            AddressSpace* address_space; 
            
            void Init();
            void Tick();
            void Frame();
            void Shutdown();

            
            // REGISTERS
            // Public so that we can inherit these for future cpus

            // Segment registers
            uint16_t cs;                // Code segment register
            uint16_t ds;                // Data segment register
            uint16_t es;                // Extra segment register
            uint16_t ss;                // Stack segment register
            
            // 16-bit registers
        
            uint16_t sp;                // Stack pointer
            uint16_t bp;                // Base pointer 
            
            uint16_t si;                // Source index         (for various e.g. "string"/arbitrary memory ops)
            uint16_t di;                // Destination index    (for various e.g. "string"/arbitrary memory ops) 

            // Instruction pointer
            uint16_t ip;                // Instruction pointer
            uint16_t pc;                // Internal register

            // Current linear address
            uint32_t linear_pc = (cs << 4) + ip;

            // Union registers
            enum CPU8086CurrentSegmentRegister
            {
                seg_override_cs = 0,
                seg_override_ds = 0,
                seg_override_es = 0,
                seg_override_ss = 0,
            };

            uint16_t* seg_current;
            uint16_t* seg_override;         // Used to implement segment override prefixes

            union 
            {
                uint16_t ax;

                struct
                {
                    uint8_t al;
                    uint8_t ah;
                };

            };

            union 
            {
                uint16_t bx;

                struct
                {
                    uint8_t bl;
                    uint8_t bh;
                };

            };

            union 
            {
                uint16_t cx;

                struct
                {
                    uint8_t cl;
                    uint8_t ch;
                };

            };

            union 
            {
                uint16_t dx;
                struct
                {
                    uint8_t dl;
                    uint8_t dh;
                };

            };

            // OVERRIDABLE
            // Apparently starting at bit 0
            struct CPU8086Flags 
            {
                bool carry : 1;         // 1 << 0
                bool reserved1 : 1;     // 1 << 1
                bool parity : 1;        // 1 << 2
                bool reserved3 : 1;     // 1 << 3
                bool auxcarry : 1;      // 1 << 4
                bool reserved5 : 1;     // 1 << 5
                bool zero : 1;          // 1 << 6
                bool sign : 1;          // 1 << 7
                bool trap : 1;          // 1 << 8
                bool intr_enable : 1;   // 1 << 9
                bool direction : 1;     // 1 << 10
                bool overflow : 1;      // 1 << 11
                bool reserved12 : 1;    // 1 << 12 
                bool reserved13 : 1;    // 1 << 13 
                bool reserved14 : 1;    // 1 << 14 
                bool reserved15 : 1;    // 1 << 15 
            };

            CPU8086Flags flags; 


            // Intel 8086 instruction encoding 
            struct CPU8086Instruction
            {
                union 
                {
                    uint8_t opcode_edw;

                    struct 
                    {
                        bool w : 1;             // Width (8/16bit)
                        bool d : 1;             // Direction bit
                        uint8_t opcode : 6;
                    };
                
                };
                
                void (CPU8086::*run_function)();

                uint8_t size;
            }; 

            struct CPU8086InstructionModRM
            {
                union
                {
                    uint8_t modrm;

                    // if needed but will try to avoid as much as possible
                    struct
                    {
                        uint8_t rm : 3;
                        uint8_t reg : 3;
                        uint8_t mod : 2;
                    };
                };

                uint16_t* ea_ptr; 

                uint8_t* reg_ptr8;
                uint16_t* reg_ptr16;
            };


        protected:
        private: 
            // Prefetch Queue
            uint16_t q0;        // UPPER 2
            uint16_t q1;        // MIDDLE 2
            uint16_t q2;        // LOWER 2
            uint8_t hl;
            bool mt;

            CPU8086::CPU8086InstructionModRM Decode_ModRM(bool w, uint8_t modrm);

            //
            // Operations
            //
            void Op_Nop();
            
            // Defined size used so that we can look up the opcode as a table
            static constexpr CPU8086Instruction instruction_table[CPU8086_NUM_OPCODES] =
            {
                { 0x00, Op_Nop, 1 }, { 0x01, Op_Nop, 1 }, { 0x02, Op_Nop, 1 },  { 0x03, Op_Nop, 1 },  { 0x04, Op_Nop, 1 }, { 0x05, Op_Nop, 1 }, { 0x06, Op_Nop, 1 },  { 0x07, Op_Nop, 1 }, 
                { 0x08, Op_Nop, 1 }, { 0x09, Op_Nop, 1 }, { 0x0A, Op_Nop, 1 },  { 0x0B, Op_Nop, 1 },  { 0x0C, Op_Nop, 1 }, { 0x0D, Op_Nop, 1 }, { 0x0E, Op_Nop, 1 },  { 0x0F, Op_Nop, 1 }, 
                { 0x10, Op_Nop, 1 }, { 0x11, Op_Nop, 1 }, { 0x12, Op_Nop, 1 },  { 0x13, Op_Nop, 1 },  { 0x14, Op_Nop, 1 }, { 0x15, Op_Nop, 1 }, { 0x16, Op_Nop, 1 },  { 0x17, Op_Nop, 1 }, 
                { 0x18, Op_Nop, 1 }, { 0x19, Op_Nop, 1 }, { 0x1A, Op_Nop, 1 },  { 0x1B, Op_Nop, 1 },  { 0x1C, Op_Nop, 1 }, { 0x1D, Op_Nop, 1 }, { 0x1E, Op_Nop, 1 },  { 0x1F, Op_Nop, 1 }, 
                { 0x20, Op_Nop, 1 }, { 0x21, Op_Nop, 1 }, { 0x22, Op_Nop, 1 },  { 0x23, Op_Nop, 1 },  { 0x24, Op_Nop, 1 }, { 0x25, Op_Nop, 1 }, { 0x26, Op_Nop, 1 },  { 0x27, Op_Nop, 1 }, 
                { 0x28, Op_Nop, 1 }, { 0x29, Op_Nop, 1 }, { 0x2A, Op_Nop, 1 },  { 0x2B, Op_Nop, 1 },  { 0x2C, Op_Nop, 1 }, { 0x2D, Op_Nop, 1 }, { 0x2E, Op_Nop, 1 },  { 0x2F, Op_Nop, 1 }, 
                { 0x30, Op_Nop, 1 }, { 0x31, Op_Nop, 1 }, { 0x32, Op_Nop, 1 },  { 0x33, Op_Nop, 1 },  { 0x34, Op_Nop, 1 }, { 0x35, Op_Nop, 1 }, { 0x36, Op_Nop, 1 },  { 0x37, Op_Nop, 1 }, 
                { 0x38, Op_Nop, 1 }, { 0x39, Op_Nop, 1 }, { 0x3A, Op_Nop, 1 },  { 0x3B, Op_Nop, 1 },  { 0x3C, Op_Nop, 1 }, { 0x3D, Op_Nop, 1 }, { 0x3E, Op_Nop, 1 },  { 0x3F, Op_Nop, 1 }, 
                { 0x40, Op_Nop, 1 }, { 0x41, Op_Nop, 1 }, { 0x42, Op_Nop, 1 },  { 0x43, Op_Nop, 1 },  { 0x44, Op_Nop, 1 }, { 0x45, Op_Nop, 1 }, { 0x46, Op_Nop, 1 },  { 0x47, Op_Nop, 1 }, 
                { 0x48, Op_Nop, 1 }, { 0x49, Op_Nop, 1 }, { 0x4A, Op_Nop, 1 },  { 0x4B, Op_Nop, 1 },  { 0x4C, Op_Nop, 1 }, { 0x4D, Op_Nop, 1 }, { 0x4E, Op_Nop, 1 },  { 0x4F, Op_Nop, 1 }, 
                { 0x50, Op_Nop, 1 }, { 0x51, Op_Nop, 1 }, { 0x52, Op_Nop, 1 },  { 0x53, Op_Nop, 1 },  { 0x54, Op_Nop, 1 }, { 0x55, Op_Nop, 1 }, { 0x56, Op_Nop, 1 },  { 0x57, Op_Nop, 1 }, 
                { 0x58, Op_Nop, 1 }, { 0x59, Op_Nop, 1 }, { 0x5A, Op_Nop, 1 },  { 0x5B, Op_Nop, 1 },  { 0x5C, Op_Nop, 1 }, { 0x5D, Op_Nop, 1 }, { 0x5E, Op_Nop, 1 },  { 0x5F, Op_Nop, 1 }, 
                { 0x60, Op_Nop, 1 }, { 0x61, Op_Nop, 1 }, { 0x62, Op_Nop, 1 },  { 0x63, Op_Nop, 1 },  { 0x64, Op_Nop, 1 }, { 0x65, Op_Nop, 1 }, { 0x66, Op_Nop, 1 },  { 0x67, Op_Nop, 1 }, 
                { 0x68, Op_Nop, 1 }, { 0x69, Op_Nop, 1 }, { 0x6A, Op_Nop, 1 },  { 0x6B, Op_Nop, 1 },  { 0x6C, Op_Nop, 1 }, { 0x6D, Op_Nop, 1 }, { 0x6E, Op_Nop, 1 },  { 0x6F, Op_Nop, 1 }, 
                { 0x70, Op_Nop, 1 }, { 0x71, Op_Nop, 1 }, { 0x72, Op_Nop, 1 },  { 0x73, Op_Nop, 1 },  { 0x74, Op_Nop, 1 }, { 0x75, Op_Nop, 1 }, { 0x76, Op_Nop, 1 },  { 0x77, Op_Nop, 1 }, 
                { 0x78, Op_Nop, 1 }, { 0x79, Op_Nop, 1 }, { 0x7A, Op_Nop, 1 },  { 0x7B, Op_Nop, 1 },  { 0x7C, Op_Nop, 1 }, { 0x7D, Op_Nop, 1 }, { 0x7E, Op_Nop, 1 },  { 0x7F, Op_Nop, 1 }, 
                { 0x80, Op_Nop, 1 }, { 0x81, Op_Nop, 1 }, { 0x82, Op_Nop, 1 },  { 0x83, Op_Nop, 1 },  { 0x84, Op_Nop, 1 }, { 0x85, Op_Nop, 1 }, { 0x86, Op_Nop, 1 },  { 0x87, Op_Nop, 1 }, 
                { 0x88, Op_Nop, 1 }, { 0x89, Op_Nop, 1 }, { 0x8A, Op_Nop, 1 },  { 0x8B, Op_Nop, 1 },  { 0x8C, Op_Nop, 1 }, { 0x8D, Op_Nop, 1 }, { 0x8E, Op_Nop, 1 },  { 0x8F, Op_Nop, 1 }, 
                { 0x90, Op_Nop, 1 }, { 0x91, Op_Nop, 1 }, { 0x92, Op_Nop, 1 },  { 0x93, Op_Nop, 1 },  { 0x94, Op_Nop, 1 }, { 0x95, Op_Nop, 1 }, { 0x96, Op_Nop, 1 },  { 0x97, Op_Nop, 1 }, 
                { 0x98, Op_Nop, 1 }, { 0x99, Op_Nop, 1 }, { 0x9A, Op_Nop, 1 },  { 0x9B, Op_Nop, 1 },  { 0x9C, Op_Nop, 1 }, { 0x9D, Op_Nop, 1 }, { 0x9E, Op_Nop, 1 },  { 0x9F, Op_Nop, 1 }, 
                { 0xA0, Op_Nop, 1 }, { 0xA1, Op_Nop, 1 }, { 0xA2, Op_Nop, 1 },  { 0xA3, Op_Nop, 1 },  { 0xA4, Op_Nop, 1 }, { 0xA5, Op_Nop, 1 }, { 0xA6, Op_Nop, 1 },  { 0xA7, Op_Nop, 1 }, 
                { 0xA8, Op_Nop, 1 }, { 0xA9, Op_Nop, 1 }, { 0xAA, Op_Nop, 1 },  { 0xAB, Op_Nop, 1 },  { 0xAC, Op_Nop, 1 }, { 0xAD, Op_Nop, 1 }, { 0xAE, Op_Nop, 1 },  { 0xAF, Op_Nop, 1 }, 
                { 0xB0, Op_Nop, 1 }, { 0xB1, Op_Nop, 1 }, { 0xB2, Op_Nop, 1 },  { 0xB3, Op_Nop, 1 },  { 0xB4, Op_Nop, 1 }, { 0xB5, Op_Nop, 1 }, { 0xB6, Op_Nop, 1 },  { 0xB7, Op_Nop, 1 }, 
                { 0xB8, Op_Nop, 1 }, { 0xB9, Op_Nop, 1 }, { 0xBA, Op_Nop, 1 },  { 0xBB, Op_Nop, 1 },  { 0xBC, Op_Nop, 1 }, { 0xBD, Op_Nop, 1 }, { 0xBE, Op_Nop, 1 },  { 0xBF, Op_Nop, 1 }, 
                { 0xC0, Op_Nop, 1 }, { 0xC1, Op_Nop, 1 }, { 0xC2, Op_Nop, 1 },  { 0xC3, Op_Nop, 1 },  { 0xC4, Op_Nop, 1 }, { 0xC5, Op_Nop, 1 }, { 0xC6, Op_Nop, 1 },  { 0xC7, Op_Nop, 1 }, 
                { 0xC8, Op_Nop, 1 }, { 0xC9, Op_Nop, 1 }, { 0xCA, Op_Nop, 1 },  { 0xCB, Op_Nop, 1 },  { 0xCC, Op_Nop, 1 }, { 0xCD, Op_Nop, 1 }, { 0xCE, Op_Nop, 1 },  { 0xCF, Op_Nop, 1 }, 
                { 0xD0, Op_Nop, 1 }, { 0xD1, Op_Nop, 1 }, { 0xD2, Op_Nop, 1 },  { 0xD3, Op_Nop, 1 },  { 0xD4, Op_Nop, 1 }, { 0xD5, Op_Nop, 1 }, { 0xD6, Op_Nop, 1 },  { 0xD7, Op_Nop, 1 }, 
                { 0xD8, Op_Nop, 1 }, { 0xD9, Op_Nop, 1 }, { 0xDA, Op_Nop, 1 },  { 0xDB, Op_Nop, 1 },  { 0xDC, Op_Nop, 1 }, { 0xDD, Op_Nop, 1 }, { 0xDE, Op_Nop, 1 },  { 0xDF, Op_Nop, 1 }, 
                { 0xE0, Op_Nop, 1 }, { 0xE1, Op_Nop, 1 }, { 0xE2, Op_Nop, 1 },  { 0xE3, Op_Nop, 1 },  { 0xE4, Op_Nop, 1 }, { 0xE5, Op_Nop, 1 }, { 0xE6, Op_Nop, 1 },  { 0xE7, Op_Nop, 1 }, 
                { 0xE8, Op_Nop, 1 }, { 0xE9, Op_Nop, 1 }, { 0xEA, Op_Nop, 1 },  { 0xEB, Op_Nop, 1 },  { 0xEC, Op_Nop, 1 }, { 0xED, Op_Nop, 1 }, { 0xEE, Op_Nop, 1 },  { 0xEF, Op_Nop, 1 }, 
                { 0xF0, Op_Nop, 1 }, { 0xF1, Op_Nop, 1 }, { 0xF2, Op_Nop, 1 },  { 0xF3, Op_Nop, 1 },  { 0xF4, Op_Nop, 1 }, { 0xF5, Op_Nop, 1 }, { 0xF6, Op_Nop, 1 },  { 0xF7, Op_Nop, 1 }, 
                { 0xF8, Op_Nop, 1 }, { 0xF9, Op_Nop, 1 }, { 0xFA, Op_Nop, 1 },  { 0xFB, Op_Nop, 1 },  { 0xFC, Op_Nop, 1 }, { 0xFD, Op_Nop, 1 }, { 0xFE, Op_Nop, 1 },  { 0xFF, Op_Nop, 1 }, 
            };

            // register table for ordering various operations
            // mod=11 and reg use the same order table!
            uint8_t* register_table8[CPU8086_NUM_REGISTERS] = { &al, &cl, &dl, &bl, &ah, &ch, &dh, &bh };
            uint16_t* register_table16[CPU8086_NUM_REGISTERS] = { &ax, &cx, &dx, &bx, &sp, &bp, &si, &di };
            
            uint16_t* rm_table[CPU8086_NUM_REGISTERS] = 
            {
                &address_space->access_word[(*seg_current << 4) + bx + si],
                &address_space->access_word[(*seg_current << 4) + bx + di],
                &address_space->access_word[(*seg_current << 4) + bp + si],
                &address_space->access_word[(*seg_current << 4) + bp + di],
                &address_space->access_word[(*seg_current << 4) + si],
                &address_space->access_word[(*seg_current << 4) + di],
                &address_space->access_word[(*seg_current << 4) + bp],
                &address_space->access_word[(*seg_current << 4) + bx],
            }

    };
}
    