#include "components/cpu/cpu8086.hpp"
#include <render/render.hpp>
#include <emulation/emulation.hpp>

namespace Volt
{
    Machine test_machine;

    void Emulation_Init()
    {
        Render_Init();

        CPU8086* cpu_808x = Memory_Alloc<CPU8086>(TAG_EMU_COMPONENT_CPU);

        test_machine.AddComponent(cpu_808x);

        // random bytes to test the prefetch queeu
        for (uint32_t i = 0; i < CPU8086_ADDR_SPACE_SIZE; i++)
        {
            cpu_808x->address_space->access_byte[i] = rand() % 255; 
        }

        cpu_808x->Start(); // this will be done by machine initialisation in the future


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
