#include "components/cpu/cpu8086.hpp"
#include <render/render.hpp>
#include <emulation/emulation.hpp>

namespace Volt
{
    Machine test_machine;

    void Emulation_Init()
    {
        Render_Init();

        CPU8086* cpu_808x = Memory_Alloc<CPU8086>(TAG_EMU_GUEST_MEM);

        test_machine.AddComponent(cpu_808x);

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
