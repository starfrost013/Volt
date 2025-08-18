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
    #define CPU8086_ADDR_SPACE_SIZE     0x100000 //1048576
    #define CPU8086_NUM_OPCODES         256
    #define CPU8086_NUM_OPCODES_PER_GRP 8
    #define CPU8086_NUM_REGISTERS       8
    #define CPU8086_PREFETCH_QUEUE_SIZE 6
    #define CPU8088_PREFETCH_QUEUE_SIZE 4
    #define CPU8086_START_LOCATION_CS   0xFFFF
    #define CPU8086_START_LOCATION_IP   0x0000

    class CPU8086 : public Component
    {
        public:
            const char* name;

            AddressSpace* address_space; 
            
            enum CPU8086Variant
            {
                cpu808x_8086 = 0x8086,
                cpu808x_8088 = 0x8088,
            };

            CPU8086Variant variant;

            void Init() override;
            void Start() override; 
            void Tick() override;
            void Frame() override;
            void Shutdown() override;

            // Is the CPU in reset?

        
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

            // TODO: Would a define be faster?
            uint32_t inline linear_pc() { return ((cs << 4) + ip) % CPU8086_ADDR_SPACE_SIZE; }

            // Union registers
            enum CPU8086CurrentSegmentRegister
            {
                seg_override_none = 0,
                seg_override_cs = 1,
                seg_override_ds = 2,
                seg_override_es = 3,
                seg_override_ss = 4,
            };

            CPU8086CurrentSegmentRegister seg_override;         // Used to implement segment override prefixes

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
            enum CPU8086Flags
            {
                Carry = 1 << 0,
                Reserved1 = 1 << 1,
                Parity = 1 << 2,
                Reserved3 = 1 << 3,
                AuxCarry = 1 << 4,
                Reserved5 = 1 << 5,
                Zero = 1 << 6,
                Sign = 1 << 7,
                Trap = 1 << 8,
                IntrEnable = 1 << 9,
                Direction = 1 << 10,
                Overflow = 1 << 11,
                Reserved12 = 1 << 12,
                Reserved13 = 1 << 13,
                Reserved14 = 1 << 14,
                Reserved15 = 1 << 15,
            };

            uint16_t flags; 

            bool halted;

            bool nmi;

           
        protected:
        // Stuff the disassembler doesn't need
        private: 

            void Disasm(uint8_t opcode);
            void Disasm_Parse(uint8_t opcode);

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
                
                void (CPU8086::*run_function)(uint8_t opcode); //opcode used for group functions and so on

                uint8_t size;
                uint8_t cycles;
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

            //
            // Operations
            //
            void Op_Nop(uint8_t opcode);                    // Do nothing
            void Op_Unimpl(uint8_t opcode);                 // Do nothing but as a placeholder for doing something

            // Control
            void Op_JmpFar(uint8_t opcode);
            void Op_ShortConditionalJmp(uint8_t opcode);
            void Op_Hlt(uint8_t opcode);

            // Flag set/clear
            void Op_Sti(uint8_t opcode);
            void Op_Cli(uint8_t opcode);
            void Op_Clc(uint8_t opcode);
            void Op_Stc(uint8_t opcode);
            void Op_Cmc(uint8_t opcode);
            void Op_Cld(uint8_t opcode);
            void Op_Std(uint8_t opcode);
            void Op_Lahf(uint8_t opcode);
            void Op_Sahf(uint8_t opcode);

            // Prefix
            void Op_DSOverridePrefix(uint8_t opcode);
            void Op_CSOverridePrefix(uint8_t opcode);
            void Op_SSOverridePrefix(uint8_t opcode);
            void Op_ESOverridePrefix(uint8_t opcode);

            // Math
            void Op_Inc(uint8_t opcode);
            void Op_Dec(uint8_t opcode);

            // Moves
            void Op_MovImmedToReg(uint8_t opcode);

            // Defined size used so that we can look up the opcode as a table
            // Disassembler needs to access this!
            static constexpr CPU8086Instruction instruction_table[CPU8086_NUM_OPCODES] =
            {
                { 0x00, Op_Unimpl, 1, 1 }, { 0x01, Op_Unimpl, 1, 1 }, { 0x02, Op_Unimpl, 1, 1 },  { 0x03, Op_Unimpl, 1, 1 },  { 0x04, Op_Unimpl, 1, 1 }, { 0x05, Op_Unimpl, 1, 1 }, { 0x06, Op_Unimpl, 1, 1 },  { 0x07, Op_Unimpl, 1, 1 }, 
                { 0x08, Op_Unimpl, 1, 1 }, { 0x09, Op_Unimpl, 1, 1 }, { 0x0A, Op_Unimpl, 1, 1 },  { 0x0B, Op_Unimpl, 1, 1 },  { 0x0C, Op_Unimpl, 1, 1 }, { 0x0D, Op_Unimpl, 1, 1 }, { 0x0E, Op_Unimpl, 1, 1 },  { 0x0F, Op_Unimpl, 1, 1 }, 
                { 0x10, Op_Unimpl, 1, 1 }, { 0x11, Op_Unimpl, 1, 1 }, { 0x12, Op_Unimpl, 1, 1 },  { 0x13, Op_Unimpl, 1, 1 },  { 0x14, Op_Unimpl, 1, 1 }, { 0x15, Op_Unimpl, 1, 1 }, { 0x16, Op_Unimpl, 1, 1 },  { 0x17, Op_Unimpl, 1, 1 }, 
                { 0x18, Op_Unimpl, 1, 1 }, { 0x19, Op_Unimpl, 1, 1 }, { 0x1A, Op_Unimpl, 1, 1 },  { 0x1B, Op_Unimpl, 1, 1 },  { 0x1C, Op_Unimpl, 1, 1 }, { 0x1D, Op_Unimpl, 1, 1 }, { 0x1E, Op_Unimpl, 1, 1 },  { 0x1F, Op_Unimpl, 1, 1 }, 
                { 0x20, Op_Unimpl, 1, 1 }, { 0x21, Op_Unimpl, 1, 1 }, { 0x22, Op_Unimpl, 1, 1 },  { 0x23, Op_Unimpl, 1, 1 },  { 0x24, Op_Unimpl, 1, 1 }, { 0x25, Op_Unimpl, 1, 1 }, { 0x26, Op_ESOverridePrefix, 1, 4 },  { 0x27, Op_Unimpl, 1, 1 }, 
                { 0x28, Op_Unimpl, 1, 1 }, { 0x29, Op_Unimpl, 1, 1 }, { 0x2A, Op_Unimpl, 1, 1 },  { 0x2B, Op_Unimpl, 1, 1 },  { 0x2C, Op_Unimpl, 1, 1 }, { 0x2D, Op_Unimpl, 1, 1 }, { 0x2E, Op_CSOverridePrefix, 1, 4 },  { 0x2F, Op_Unimpl, 1, 1 }, 
                { 0x30, Op_Unimpl, 1, 1 }, { 0x31, Op_Unimpl, 1, 1 }, { 0x32, Op_Unimpl, 1, 1 },  { 0x33, Op_Unimpl, 1, 1 },  { 0x34, Op_Unimpl, 1, 1 }, { 0x35, Op_Unimpl, 1, 1 }, { 0x36, Op_SSOverridePrefix, 1, 4 },  { 0x37, Op_Unimpl, 1, 1 }, 
                { 0x38, Op_Unimpl, 1, 1 }, { 0x39, Op_Unimpl, 1, 1 }, { 0x3A, Op_Unimpl, 1, 1 },  { 0x3B, Op_Unimpl, 1, 1 },  { 0x3C, Op_Unimpl, 1, 1 }, { 0x3D, Op_Unimpl, 1, 1 }, { 0x3E, Op_DSOverridePrefix, 1, 4 },  { 0x3F, Op_Unimpl, 1, 1 }, 
                { 0x40, Op_Unimpl, 1, 1 }, { 0x41, Op_Unimpl, 1, 1 }, { 0x42, Op_Unimpl, 1, 1 },  { 0x43, Op_Unimpl, 1, 1 },  { 0x44, Op_Unimpl, 1, 1 }, { 0x45, Op_Unimpl, 1, 1 }, { 0x46, Op_Unimpl, 1, 1 },  { 0x47, Op_Unimpl, 1, 1 }, 
                { 0x48, Op_Unimpl, 1, 1 }, { 0x49, Op_Unimpl, 1, 1 }, { 0x4A, Op_Unimpl, 1, 1 },  { 0x4B, Op_Unimpl, 1, 1 },  { 0x4C, Op_Unimpl, 1, 1 }, { 0x4D, Op_Unimpl, 1, 1 }, { 0x4E, Op_Unimpl, 1, 1 },  { 0x4F, Op_Unimpl, 1, 1 }, 
                { 0x50, Op_Unimpl, 1, 1 }, { 0x51, Op_Unimpl, 1, 1 }, { 0x52, Op_Unimpl, 1, 1 },  { 0x53, Op_Unimpl, 1, 1 },  { 0x54, Op_Unimpl, 1, 1 }, { 0x55, Op_Unimpl, 1, 1 }, { 0x56, Op_Unimpl, 1, 1 },  { 0x57, Op_Unimpl, 1, 1 }, 
                { 0x58, Op_Unimpl, 1, 1 }, { 0x59, Op_Unimpl, 1, 1 }, { 0x5A, Op_Unimpl, 1, 1 },  { 0x5B, Op_Unimpl, 1, 1 },  { 0x5C, Op_Unimpl, 1, 1 }, { 0x5D, Op_Unimpl, 1, 1 }, { 0x5E, Op_Unimpl, 1, 1 },  { 0x5F, Op_Unimpl, 1, 1 }, 
                { 0x60, Op_ShortConditionalJmp, 2, 1 }, { 0x61, Op_ShortConditionalJmp, 2, 1 }, { 0x62, Op_ShortConditionalJmp, 2, 1 },  { 0x63, Op_ShortConditionalJmp, 2, 1 },  { 0x64, Op_ShortConditionalJmp, 2, 1 }, { 0x65, Op_ShortConditionalJmp, 2, 1 }, { 0x66, Op_ShortConditionalJmp, 2, 1 },  { 0x67, Op_ShortConditionalJmp, 2, 1 }, 
                { 0x68, Op_ShortConditionalJmp, 2, 1 }, { 0x69, Op_ShortConditionalJmp, 2, 1 }, { 0x6A, Op_ShortConditionalJmp, 2, 1 },  { 0x6B, Op_ShortConditionalJmp, 2, 1 },  { 0x6C, Op_ShortConditionalJmp, 2, 1 }, { 0x6D, Op_ShortConditionalJmp, 2, 1 }, { 0x6E, Op_ShortConditionalJmp, 2, 1 },  { 0x6F, Op_ShortConditionalJmp, 2, 1 }, 
                { 0x70, Op_ShortConditionalJmp, 2, 1 }, { 0x71, Op_ShortConditionalJmp, 2, 1 }, { 0x72, Op_ShortConditionalJmp, 2, 1 },  { 0x73, Op_ShortConditionalJmp, 2, 1 },  { 0x74, Op_ShortConditionalJmp, 2, 1 }, { 0x75, Op_ShortConditionalJmp, 2, 1 }, { 0x76, Op_ShortConditionalJmp, 2, 1 },  { 0x77, Op_ShortConditionalJmp, 2, 1 }, 
                { 0x78, Op_ShortConditionalJmp, 2, 1 }, { 0x79, Op_ShortConditionalJmp, 2, 1 }, { 0x7A, Op_ShortConditionalJmp, 2, 1 },  { 0x7B, Op_ShortConditionalJmp, 2, 1 },  { 0x7C, Op_ShortConditionalJmp, 2, 1 }, { 0x7D, Op_ShortConditionalJmp, 2, 1 }, { 0x7E, Op_ShortConditionalJmp, 2, 1 },  { 0x7F, Op_ShortConditionalJmp, 2, 1 }, 
                { 0x80, Op_Unimpl, 1, 1 }, { 0x81, Op_Unimpl, 1, 1 }, { 0x82, Op_Unimpl, 1, 1 },  { 0x83, Op_Unimpl, 1, 1 },  { 0x84, Op_Unimpl, 1, 1 }, { 0x85, Op_Unimpl, 1, 1 }, { 0x86, Op_Unimpl, 1, 1 },  { 0x87, Op_Unimpl, 1, 1 }, 
                { 0x88, Op_Unimpl, 1, 1 }, { 0x89, Op_Unimpl, 1, 1 }, { 0x8A, Op_Unimpl, 1, 1 },  { 0x8B, Op_Unimpl, 1, 1 },  { 0x8C, Op_Unimpl, 1, 1 }, { 0x8D, Op_Unimpl, 1, 1 }, { 0x8E, Op_Unimpl, 1, 1 },  { 0x8F, Op_Unimpl, 1, 1 }, 
                { 0x90, Op_Nop, 1, 1 }, { 0x91, Op_Unimpl, 1, 1 }, { 0x92, Op_Unimpl, 1, 1 },  { 0x93, Op_Unimpl, 1, 1 },  { 0x94, Op_Unimpl, 1, 1 }, { 0x95, Op_Unimpl, 1, 1 }, { 0x96, Op_Unimpl, 1, 1 },  { 0x97, Op_Unimpl, 1, 1 }, 
                { 0x98, Op_Unimpl, 1, 1 }, { 0x99, Op_Unimpl, 1, 1 }, { 0x9A, Op_Unimpl, 1, 1 },  { 0x9B, Op_Unimpl, 1, 1 },  { 0x9C, Op_Unimpl, 1, 1 }, { 0x9D, Op_Unimpl, 1, 1 }, { 0x9E, Op_Sahf, 1, 1 },  { 0x9F, Op_Lahf, 1, 1 }, 
                { 0xA0, Op_Unimpl, 1, 1 }, { 0xA1, Op_Unimpl, 1, 1 }, { 0xA2, Op_Unimpl, 1, 1 },  { 0xA3, Op_Unimpl, 1, 1 },  { 0xA4, Op_Unimpl, 1, 1 }, { 0xA5, Op_Unimpl, 1, 1 }, { 0xA6, Op_Unimpl, 1, 1 },  { 0xA7, Op_Unimpl, 1, 1 }, 
                { 0xA8, Op_Unimpl, 1, 1 }, { 0xA9, Op_Unimpl, 1, 1 }, { 0xAA, Op_Unimpl, 1, 1 },  { 0xAB, Op_Unimpl, 1, 1 },  { 0xAC, Op_Unimpl, 1, 1 }, { 0xAD, Op_Unimpl, 1, 1 }, { 0xAE, Op_Unimpl, 1, 1 },  { 0xAF, Op_Unimpl, 1, 1 }, 
                { 0xB0, Op_MovImmedToReg, 2, 1 }, { 0xB1, Op_MovImmedToReg, 2, 1 }, { 0xB2, Op_MovImmedToReg, 2, 1 },  { 0xB3, Op_MovImmedToReg, 2, 1 },  { 0xB4, Op_MovImmedToReg, 2, 1 }, { 0xB5, Op_MovImmedToReg, 2, 1 }, { 0xB6, Op_MovImmedToReg, 2, 1 },  { 0xB7, Op_MovImmedToReg, 2, 1 }, 
                { 0xB8, Op_MovImmedToReg, 3, 1 }, { 0xB9, Op_MovImmedToReg, 3, 1 }, { 0xBA, Op_MovImmedToReg, 3, 1 },  { 0xBB, Op_MovImmedToReg, 3, 1 },  { 0xBC, Op_MovImmedToReg, 3, 1 }, { 0xBD, Op_MovImmedToReg, 3, 1 }, { 0xBE, Op_MovImmedToReg, 3, 1 },  { 0xBF, Op_MovImmedToReg, 3, 1 }, 
                { 0xC0, Op_Unimpl, 1, 1 }, { 0xC1, Op_Unimpl, 1, 1 }, { 0xC2, Op_Unimpl, 1, 1 },  { 0xC3, Op_Unimpl, 1, 1 },  { 0xC4, Op_Unimpl, 1, 1 }, { 0xC5, Op_Unimpl, 1, 1 }, { 0xC6, Op_Unimpl, 1, 1 },  { 0xC7, Op_Unimpl, 1, 1 }, 
                { 0xC8, Op_Unimpl, 1, 1 }, { 0xC9, Op_Unimpl, 1, 1 }, { 0xCA, Op_Unimpl, 1, 1 },  { 0xCB, Op_Unimpl, 1, 1 },  { 0xCC, Op_Unimpl, 1, 1 }, { 0xCD, Op_Unimpl, 1, 1 }, { 0xCE, Op_Unimpl, 1, 1 },  { 0xCF, Op_Unimpl, 1, 1 }, 
                { 0xD0, Op_Unimpl, 1, 1 }, { 0xD1, Op_Unimpl, 1, 1 }, { 0xD2, Op_Unimpl, 1, 1 },  { 0xD3, Op_Unimpl, 1, 1 },  { 0xD4, Op_Unimpl, 1, 1 }, { 0xD5, Op_Unimpl, 1, 1 }, { 0xD6, Op_Unimpl, 1, 1 },  { 0xD7, Op_Unimpl, 1, 1 }, 
                { 0xD8, Op_Unimpl, 1, 1 }, { 0xD9, Op_Unimpl, 1, 1 }, { 0xDA, Op_Unimpl, 1, 1 },  { 0xDB, Op_Unimpl, 1, 1 },  { 0xDC, Op_Unimpl, 1, 1 }, { 0xDD, Op_Unimpl, 1, 1 }, { 0xDE, Op_Unimpl, 1, 1 },  { 0xDF, Op_Unimpl, 1, 1 }, 
                { 0xE0, Op_Unimpl, 1, 1 }, { 0xE1, Op_Unimpl, 1, 1 }, { 0xE2, Op_Unimpl, 1, 1 },  { 0xE3, Op_Unimpl, 1, 1 },  { 0xE4, Op_Unimpl, 1, 1 }, { 0xE5, Op_Unimpl, 1, 1 }, { 0xE6, Op_Unimpl, 1, 1 },  { 0xE7, Op_Unimpl, 1, 1 }, 
                { 0xE8, Op_Unimpl, 1, 1 }, { 0xE9, Op_Unimpl, 1, 1 }, { 0xEA, Op_JmpFar, 5, 1 },  { 0xEB, Op_Unimpl, 1, 1 },  { 0xEC, Op_Unimpl, 1, 1 }, { 0xED, Op_Unimpl, 1, 1 }, { 0xEE, Op_Unimpl, 1, 1 },  { 0xEF, Op_Unimpl, 1, 1 }, 
                { 0xF0, Op_Unimpl, 1, 1 }, { 0xF1, Op_Unimpl, 1, 1 }, { 0xF2, Op_Unimpl, 1, 1 },  { 0xF3, Op_Unimpl, 1, 1 },  { 0xF4, Op_Hlt, 1, 1 }, { 0xF5, Op_Cmc, 1, 1 }, { 0xF6, Op_Unimpl, 1, 1 },  { 0xF7, Op_Unimpl, 1, 1 }, 
                { 0xF8, Op_Clc, 1, 1 }, { 0xF9, Op_Stc, 1, 1 }, { 0xFA, Op_Cli, 1, 1 },  { 0xFB, Op_Sti, 1, 1 },  { 0xFC, Op_Cld, 1, 1 }, { 0xFD, Op_Std, 1, 1 }, { 0xFE, Op_Unimpl, 1, 1 },  { 0xFF, Op_Unimpl, 1, 1 }, 
            };

            // Disassembler table.
            // Note: "GRP1,2,3,4,5" stuff is not listed here (as well as prefixes), it is built dynamically during parsing
            // We don't use the reg tables to simplify stuff for the disassembler
            static constexpr const char* opcode_table_disasm[CPU8086_NUM_OPCODES] =
            {
                "ADD", "ADD", "ADD", "ADD", "ADD AL, ", "ADD AX, ", "PUSH ES", "POP ES",
                "OR", "OR", "OR", "OR", "OR", "OR", "PUSH CS", "POP CS [INVALID]",
                "ADC", "ADC", "ADC", "ADC", "ADC AL, ", "ADC AX, ", "PUSH SS", "POP SS",
                "SBB", "SBB", "SBB", "SBB", "SBB AL, ", "SBB AX, ", "PUSH DS", "POP DS",
                "AND", "AND", "AND", "AND", "AND AL, ","AND AX, ", "", "DAA",
                "SUB", "SUB", "SUB", "SUB", "SUB AL, ","SUB AX, ", "", "DAS",
                //TODO: REST OF TABLE
                "XOR", "XOR", "XOR", "XOR", "XOR AL, ", "XOR AX, ", "" , "AAA",
                "CMP", "CMP", "CMP", "CMP", "CMP AL, ", "CMP AX, ", "" , "AAS",
                "INC AX", "INC CX", "INC DX", "INC BX", "INC SP", "INC BP", "INC SI", "INC DI", 
                "DEC AX", "DEC CX", "DEC DX", "DEC BX", "DEC SP", "DEC BP", "DEC SI", "DEC DI", 
                "PUSH AX", "PUSH CX", "PUSH DX", "PUSH BX", "PUSH SP", "PUSH BP", "PUSH SI", "PUSH DI",
                "POP AX", "POP CX", "POP DX", "POP BX", "POP SP", "POP BP", "POP SI", "POP DI", 
                // 6x = 7x
                "JO", "JNO", "JB", "JNB", "JZ/JE", "JNZ/JNE", "JBE", "JA",
                "JS", "JNS", "JPE", "JPO", "JL", "JGE", "JLE", "JG",
                "JO", "JNO", "JB", "JNB", "JZ/JE", "JNZ/JNE", "JBE", "JA",
                "JS", "JNS", "JPE", "JPO", "JL", "JGE", "JLE", "JG",
                "", "", "", "", "TEST", "TEST", "XCHG", "XCHG", //GRP1 - Use reg to print
                "MOV", "MOV", "MOV", "MOV", "MOV", "LEA", "MOV", "POP",
                "NOP", "XCHG", "XCHG", "XCHG", "XCHG", "XCHG", "XCHG", "XCHG", // use register_table16, AX
                "CBW", "CMD", "CALL", "WAIT", "PUSHF", "POPF", "SAHF", "LAHF",
                "MOV AL, ", "MOV AX, ", "MOV", "MOV", "MOVSB", "MOVSW", "CMPSB", "CMPSW", // use offset
                "TEST AL, ", "TEST AX, ", "STOSB", "STOSW", "LODSB", "LODSW", "SCASB", "SCASW",
                "MOV", "MOV", "MOV", "MOV", "MOV", "MOV", "MOV", "MOV", // use register_table8
                "MOV", "MOV", "MOV", "MOV", "MOV", "MOV", "MOV", "MOV", // use register_table16
                "RET [!]", "RET", "RET", "RET", "LES", "LDS", "MOV", "MOV",
                "RETF [!]", "RETF [!]", "RETF", "RETF", "INT3", "INT", "INTO", "IRET", //GRP2 - Use reg to print
                "", "", "", "", "AAM", "AAD", "SALC", "XLAT",
                "x87 ESC", "x87 ESC", "x87 ESC", "x87 ESC", "x87 ESC", "x87 ESC", "x87 ESC", "x87 ESC", 
                "LOOPNZ", "LOOPZ", "LOOP", "JCXZ", "IN", "IN", "OUT", "OUT", 
                "CALL", "JMP", "JMP", "JMP", "IN", "IN", "OUT", "OUT", 
                "LOCK", "LOCK [!]", "REPNZ", "REPZ", "HLT", "CMC", "", "", //GRP3 - Use reg to print
                "CLC", "STC", "CLI", "STI", "CLD", "STD", "", ""  //GRP4/5 - Use reg to print
            };

            // tables used for disasm grp1-grp5
            static constexpr const char* grp1_table_disasm[CPU8086_NUM_OPCODES_PER_GRP] = {"ADD", "OR", "ADC", "SBB", "AND", "SUB", "XOR", "OR"};
            static constexpr const char* grp2_table_disasm[CPU8086_NUM_OPCODES_PER_GRP] = {"ROL", "ROR", "RCL", "RCR", "SHL", "SHR", "SETMO", "SAR"};
            static constexpr const char* grp3_table_disasm[CPU8086_NUM_OPCODES_PER_GRP] = {"TEST", "TEST [!]", "NOT", "NEG", "MUL", "IMUL", "DIV", "IDIV"};
            static constexpr const char* grp4_table_disasm[CPU8086_NUM_OPCODES_PER_GRP] = {"INC", "DEC", "CALLB [!]", "CALLB [!]", "JMPB [!]", "JMPB [!]", "PUSHB [!]", "PUSHB [!]"};
            static constexpr const char* grp5_table_disasm[CPU8086_NUM_OPCODES_PER_GRP] = {"INC", "DEC", "CALL", "CALL", "JMP", "JMP", "PUSH", "PUSH"};

            static constexpr const char* register_table8_disasm[CPU8086_NUM_REGISTERS] = { "AL", "CL", "DL", "BL", "AH", "CH", "DH", "BH" };
            static constexpr const char* register_table16_disasm[CPU8086_NUM_REGISTERS] = { "AX", "CX", "DX", "BX", "SP", "BP", "SI", "DI" };
            static constexpr const char* rm_table_disasm[CPU8086_NUM_REGISTERS] = {"[BX + SI", "[BX + DI", "[BP + SI", "[BP + DI", "[SI", "[DI", "[BP", "[BX"};
        
            //
            // Prefetch Queue (basic impl.)
            //

            uint16_t prefetch[CPU8086_PREFETCH_QUEUE_SIZE];
            // this only goes up to 6 anyway. int8_t for MATHEMATICAL REASONS. DO NOT CHANGE.
            int8_t prefetch_ptr; 
            
            uint8_t Prefetch_Pop8();
            uint16_t Prefetch_Pop16();
            void Prefetch_Advance(uint32_t size);
            void Prefetch_Flush();

            // 
            // Decode
            //

            CPU8086::CPU8086InstructionModRM Decode_ModRM(bool w, uint8_t modrm);

            void inline SetPZSFlags8(uint8_t result);
            void inline SetPZSFlags16(uint16_t result);
            void inline SetOF8_Add(uint8_t result, uint8_t old_result, uint8_t operand);
            void inline SetOF8_Sub(uint8_t result, uint8_t old_result, uint8_t operand);
            void inline SetOF16_Add(uint8_t result, uint8_t old_result, uint8_t operand);
            void inline SetOF16_Sub(uint8_t result, uint8_t old_result, uint8_t operand);

            // register table for ordering various operations
            // mod=11 and reg use the same order table!
            uint8_t* register_table8[CPU8086_NUM_REGISTERS] = { &al, &cl, &dl, &bl, &ah, &ch, &dh, &bh };
            uint16_t* register_table16[CPU8086_NUM_REGISTERS] = { &ax, &cx, &dx, &bx, &sp, &bp, &si, &di };
            
            uint16_t rm_table[CPU8086_NUM_REGISTERS] = 
            {
                (uint16_t)(bx + si),
                (uint16_t)(bx + di),
                (uint16_t)(bp + si),
                (uint16_t)(bp + di),
                si,
                di,
                bp,
                bx,
            };

    };
}
    