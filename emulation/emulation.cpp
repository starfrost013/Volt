#include "components/cpu/cpu8086.hpp"
#include <render/render.hpp>
#include <emulation/emulation.hpp>

namespace Volt
{
    Machine test_machine;
    //TEMP
    void Emulation_Init()
    {
        Render_Init();

        Logging_LogAll("******** Emulation_Init ********", LogChannel::Debug);

        CPU8086* cpu_808x = Memory_Alloc<CPU8086>(TAG_EMU_COMPONENT_CPU);
        test_machine.AddComponent(cpu_808x);
    }
    
    void Emulation_Start()
    {
        test_machine.Start();
    }

    void Emulation_Frame()
    {
        Render_Frame();
    }

    void Emulation_Tick()
    {
        // Todo: Clocking
        test_machine.Tick();
    }

    void Emulation_Shutdown()
    {
        Render_Shutdown(); 
    }
}
