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

            // Union registers

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
            };

        protected:
        private: 
            // Prefetch Queue
            uint16_t q0;        // UPPER 2
            uint16_t q1;        // MIDDLE 2
            uint16_t q2;        // LOWER 2
            uint8_t hl;
            bool mt;

            CPU8086::CPU8086InstructionModRM Decode_ModRM(bool w, uint32_t modrm);

            //
            // Operations
            //
            void Op_Nop();
            
            // Defined size used so that we can look up the opcode as a table
            static constexpr CPU8086Instruction instruction_table[CPU8086_NUM_OPCODES] =
            {
                { 0x90, Op_Nop, 1 },
            };
    };
}
    