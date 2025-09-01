
// Yes this will have to include every single header file for every component
#include <emulation/emulation.hpp>
#include "components/cpu/cpu8086.hpp"
#include "components/support/i8237_dma.hpp"
#include "components/support/i8253_pit.hpp"
#include "components/support/i8255_ppi.hpp"
#include "components/support/i8259_pic.hpp"

// BIOS
#include "components/bios/bios_xt.hpp"

namespace Volt
{

    template <typename T>
    Component* Emulation_CreateComponent()
    {
        T* new_comp = Memory_Alloc<T>(TAG_EMU_COMPONENT_GEN);
        emu_machine.AddComponent<T>(new_comp);
        return new_comp; 
    }

    std::unordered_map<std::string_view, Component*(*)()> component_creation_map =
    {
        { "cpu8086", &Emulation_CreateComponent<CPU8086> },
        { "dma8237", &Emulation_CreateComponent<DMA8237> },
        { "pit8253", &Emulation_CreateComponent<PIT8253> },
        { "ppi8255", &Emulation_CreateComponent<PPI8255> },
        { "pic8259", &Emulation_CreateComponent<PIC8259> },
        { "biosxtv3", &Emulation_CreateComponent<BIOSXTv3> },
    };

    void Emulation_Cmd_MachineAddComponent(Volt::CommandType type)
    {
        const char* key = Command_Argv(1);

        auto search = component_creation_map.find(key);

        if (search != component_creation_map.end())
            search->second();
    }


    void Emulation_InitCommands()
    {
        Command_Add("machine_add_component", CommandType::GlobalCommand, Emulation_Cmd_MachineAddComponent);
    }
}