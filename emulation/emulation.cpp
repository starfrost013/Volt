#include <render/render.hpp>
#include <emulation/emulation.hpp>

namespace Volt
{
    void Emulation_Init()
    {
        Render_Init();
    }

    void Emulation_Frame()
    {
        Render_Frame();
    }

    void Emulation_Tick()
    {
        
    }

    void Emulation_Shutdown()
    {
        Render_Shutdown(); 
    }
}
