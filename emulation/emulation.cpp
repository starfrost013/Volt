
#include <emulation/emulation.hpp>
#include "components/cpu/cpu8086.hpp"
#include "components/support/i8237_dma.hpp"
#include "components/support/i8253_pit.hpp"
#include "components/support/i8255_ppi.hpp"
#include <emulation/components/io/io_port.hpp>
#include <render/render.hpp>

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
        DMA8237* dma = Memory_Alloc<DMA8237>(TAG_EMU_COMPONENT_DMA);
        PPI8255* ppi = Memory_Alloc<PPI8255>(TAG_EMU_COMPONENT_PERIPHERAL);

        test_machine.AddComponent(cpu_808x);
        test_machine.AddComponent(pit);
        test_machine.AddComponent(dma);
        test_machine.AddComponent(ppi);

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
        test_machine.Shutdown();
        IOx86_Shutdown();           // kill IOx86 last just in case
    }
}
