// 
// VOLT
// Copyright © 2025 starfrost
//
// bios_xt.hpp: XTv1, v2, v3 BIOS
//

#include <emulation/emulation.hpp>
#include <emulation/components/cpu/cpu8086.hpp>

namespace Volt
{
    Cvar* emu_bios_xtv3_path_low;
    Cvar* emu_bios_xtv3_path_high;

    #define BIOSXTV3_HIGH_START     0xF8000

    // BIOS for IBM PC XT, version 3
    class BIOSXTv3 : public Component
    {
        public: 

            void Init(Machine* machine_ptr) override
            {
                machine = machine_ptr;
                update = false; 
                allow_device_writes = false; 
                allow_device_reads = true; 
                early_start = true; 
                name = "IBM XT BIOS Version 3 (May 1986)";
            }
        
            void Start() override
            {
                emu_bios_xtv3_path_low = Cvar_Get("emu_bios_xtv3_path_low", "BIOS_5160_09MAY86_U19_62X0819_68X4370_27256_F000.BIN", false);
                emu_bios_xtv3_path_high = Cvar_Get("emu_bios_xtv3_path_high", "BIOS_5160_09MAY86_U18_59X7268_62X0890_27256_F800.BIN", false);

                bios_low = Filesystem_OpenFile(emu_bios_xtv3_path_low->string, VoltFileMode::Binary);
                bios_high = Filesystem_OpenFile(emu_bios_xtv3_path_high->string, VoltFileMode::Binary);

                // here so the order doesn't matter
                address_space_primary->AddMapping(0xF0000, 0xFFFFF, this);

                // componentread* is for e.g. PCI BARs
                bios_low->file.read((char*)&address_space_primary->access_byte[0xF0000], 32768);
                bios_high->file.read((char*)&address_space_primary->access_byte[0xF8000], 32768);

                // we don't need them anymore so we can close them
                Filesystem_CloseFile(bios_low);
                Filesystem_CloseFile(bios_high);

            }

            void Tick() override { };        // Not used in this component
            void Frame() override { };       // Not used in this component
            
            void Shutdown() override
            {
                
            }

        private:
            VoltFile* bios_low;
            VoltFile* bios_high; 

    };
}