#include "components/cpu/cpu8086.hpp"
#include "components/support/i8253_pit.hpp"
#include <render/render.hpp>
#include <emulation/emulation.hpp>

namespace Volt
{
    Machine test_machine;

    bool emulation_running; 

    //TEMP
    void Emulation_Init()
    {
        Render_Init();

        Logging_LogAll("******** Emulation_Init ********", LogChannel::Debug);

        CPU8086* cpu_808x = Memory_Alloc<CPU8086>(TAG_EMU_COMPONENT_CPU);
        PIT8253* pit = Memory_Alloc<PIT8253>(TAG_EMU_COMPONENT_TIMER);
        test_machine.AddComponent(cpu_808x);
        test_machine.AddComponent(pit);

        emulation_running = true; 
        
    }
    
    void Emulation_Start()
    {
        test_machine.Start();
    }

    // ENTRY POINT OF THE EMU_THREAD!
    void Emulation_Tick()
    {
        while (emulation_running)
            test_machine.Tick();
    }

    void Emulation_Shutdown()
    {
        Render_Shutdown(); 
    }
}
