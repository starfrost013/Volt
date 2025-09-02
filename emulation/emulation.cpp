#include <emulation/emulation.hpp>
#include "components/cpu/cpu8086.hpp"
#include "components/support/i8237_dma.hpp"
#include "components/support/i8253_pit.hpp"
#include "components/support/i8255_ppi.hpp"
#include <emulation/components/io/io_port.hpp>
#include <render/render.hpp>

namespace Volt
{
    Machine emu_machine;

    bool emulation_running; 

    // TODO: Is this the best place for these?
    Cvar* emu_system_ram; 

    //TEMP
    void Emulation_Init()
    {
        Logging_LogAll("******** Emulation_Init ********", LogChannel::Debug);

        Emulation_InitCommands();

        Render_Init();

        emu_system_ram = Cvar_Get("emu_system_ram", "655360", false);

        emulation_running = true; 
    }
    
    void Emulation_Start()
    {
        //TODO: Automatically create a default config file, and go on
        if (emu_machine.components.size() == 0)
            Logging_LogChannel("Provide a config file!\n", LogChannel::Fatal);
        else
            emu_machine.Start();
    }

    // ENTRY POINT OF THE EMU_THREAD!
    void Emulation_Tick()
    {
        while (emulation_running)
            emu_machine.Tick();
    }

    void Emulation_Shutdown()
    {
        // Stop the emulation thread. 
        emulation_running = false;
        
        // However, it may complete up to one instruction before terminating...So wait for it to do that.
        emu_thread->join();

        emu_machine.Shutdown();
        IOx86_Shutdown();           // kill IOx86 last just in case

        // TODO: Should this be somewhere else?
        Render_Shutdown();

    }
}
