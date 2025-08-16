// 
// VOLT
// Copyright © 2025 starfrost
//
// cpu8086_core.cpp: 8086 core implementation
//
#include "cpu8086.hpp"
#include <emulation/emulation.hpp>
#include <emulation/components/cpu/cpu8086.hpp>

namespace Volt
{

    // TEMP convar
    Cvar* emu_clk_8086;

    void CPU8086::Init()
    {
        // cannot use constructor here due to MemAlloc limitations
        if (!variant)
            variant = CPU8086Variant::cpu808x_8086;

        switch (variant)
        {
            case CPU8086Variant::cpu808x_8086:
                name = "[1978] Intel 8086 CPU";
                break;
            case CPU8086Variant::cpu808x_8088:
                name = "[1979] Intel 8088 CPU";
                break; 
            default:
                name = "Intel 808x Family CPU";
                break;
        }

        Logging_LogChannel("8086: Initialising address space (variant: %s)", LogChannel::Debug, name);

        //add primary address space
        address_space = AddressSpace_Add<CPU8086_ADDR_SPACE_SIZE>();
        
        emu_clk_8086 = Cvar_Get("emu_clk_8086", "4772726", false);

        clock_hz = uint64_t(emu_clk_8086->value);
        
        Logging_LogChannel("8086: Clock speed = %d Hz", LogChannel::Debug, clock_hz);

        update = true; 
    }

    // Starts the CPU.
    // The start method is run after all components have initialised.
    void CPU8086::Start()
    {
        // Open the IBM PC 5160 BIOS as a test
        //TODO: MACHINE CONFIG, BIOS CONFIG, ANTI-86BOX CONFIG!
        VoltFile* bios_low = Filesystem_OpenFile("BIOS_5160_09MAY86_U19_62X0819_68X4370_27256_F000.BIN", VoltFileMode::Binary);
        VoltFile* bios_high = Filesystem_OpenFile("BIOS_5160_09MAY86_U18_59X7268_62X0890_27256_F800.BIN", VoltFileMode::Binary);

        // this is very bad and slow make a bulk read function
        bios_low->file.read((char*)&address_space->access_byte[0xF0000], 32768);
        bios_high->file.read((char*)&address_space->access_byte[0xF8000], 32768);
    
        cs = CPU8086_START_LOCATION_CS;
        ip = CPU8086_START_LOCATION_IP;

        //read in 6 bytes of prefetch so we can start executing 
        Prefetch_Flush();
    }

    //
    // PREFETCH QUEUE IMPLEMENTATION
    //

    uint8_t CPU8086::Prefetch_Pop8()
    {
        uint8_t prefetch_size = (variant == CPU8086Variant::cpu808x_8088) ? CPU8088_PREFETCH_QUEUE_SIZE : CPU8086_PREFETCH_QUEUE_SIZE;

        // ++ is the direction that the real 8086 does
        uint8_t ret = prefetch[prefetch_ptr];
        prefetch_ptr++;

        prefetch_ptr %= prefetch_size;
        // read in another byte

        return ret; 
    }

    uint16_t CPU8086::Prefetch_Pop16()
    {
        uint8_t prefetch_size = (variant == CPU8086Variant::cpu808x_8088) ? CPU8088_PREFETCH_QUEUE_SIZE : CPU8086_PREFETCH_QUEUE_SIZE;

        //prefetch_ptr guaranteed to never exceed prefetch_size but +1 might
        uint16_t ret = prefetch[(prefetch_ptr + 1) % prefetch_size] << 8 | prefetch[prefetch_ptr];
        prefetch_ptr += 2;
    
        if (variant == CPU8086Variant::cpu808x_8086)
            prefetch_ptr %= CPU8086_PREFETCH_QUEUE_SIZE;
        else
            prefetch_ptr %= CPU8088_PREFETCH_QUEUE_SIZE;

        return ret; 
    }

    void CPU8086::Prefetch_Advance(uint32_t size)
    {
        uint8_t prefetch_size = (variant == CPU8086Variant::cpu808x_8088) ? CPU8088_PREFETCH_QUEUE_SIZE : CPU8086_PREFETCH_QUEUE_SIZE;

        // ensure a valid size
        if (size > prefetch_size) 
            size = prefetch_size;

        // goes in reverse like real 8086

        // move everything "back" so that we can advance the queue. We don't care about stuff > size. We don't care about stuff > size. If the size is prefetch_size, 
        // we are throwing everything away anyway so don't bother
        // Example: 1 byte opcode: preserve 5 elements, drop the last
        // 0 1 2 3 4 5 -> 1 2 3 4 5 0
        if (size < prefetch_size)
        {
            for (int32_t i = 0; i <= (prefetch_size - size); i++)
                prefetch[i % prefetch_size] = prefetch[(i + size) % prefetch_size];
        }

        for (int32_t i = (prefetch_size - size); i < prefetch_size; i++)
            prefetch[i] = address_space->access_byte[linear_pc() + i];

        prefetch_ptr -= size; 
        
        //t his is terrible but make the queue CIRCULAR
        if (prefetch_ptr < 0)
            prefetch_ptr = (prefetch_size - abs(prefetch_ptr));

        Logging_LogChannel("Prefetch is: %02x %02x %02x %02x %02x %02x", LogChannel::Debug, prefetch[0], prefetch[1], prefetch[2], prefetch[3], prefetch[4], prefetch[5]);
    }

    void CPU8086::Prefetch_Flush()
    {
        uint8_t prefetch_size = (variant == CPU8086Variant::cpu808x_8088) ? CPU8088_PREFETCH_QUEUE_SIZE : CPU8086_PREFETCH_QUEUE_SIZE;

        for (int32_t i = 0; i < prefetch_size; i++)
            prefetch[i] = address_space->access_byte[linear_pc() + i];

        Logging_LogChannel("Prefetch flush: %02x %02x %02x %02x %02x %02x", LogChannel::Debug, prefetch[0], prefetch[1], prefetch[2], prefetch[3], prefetch[4], prefetch[5]);

        prefetch_ptr = 0;

        prefetch_flushed = true; 
    }

    void CPU8086::Tick()
    {
        //TODO: PREFETCH QUEUE IMPLEMENTATION

        // Keep the prefetch queue filled up
        uint8_t opcode = Prefetch_Pop8();

        if (instruction_table[opcode].run_function)
            (this->*instruction_table[opcode].run_function)();

        ip += instruction_table[opcode].size;
        clock_skip = instruction_table[opcode].cycles;
        
        if (!prefetch_flushed)
            Prefetch_Advance(instruction_table[opcode].size); 

        prefetch_flushed = false; 

        //Logging_LogAll("808x: cs=%04x ip=%04x", cs, ip);

        ip &= 0xFFFF; // wrap around

        //if we reached here there are no more segment prefixes
        // so reset
        seg_override = seg_override_none;
    }

    void CPU8086::Frame()
    {
        //not used for CPUs. Only for some video cards...
    }

    void CPU8086::Shutdown()
    {

    }

    //
    // OPcodes (move to own file)
    //

    void CPU8086::Op_Nop()
    {

    }
}