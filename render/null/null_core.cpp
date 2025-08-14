#include <render/null/render_null.hpp>

namespace Volt
{
    /* Globals */
    NullRendererState null_render_state;

    // Initialises the GL3 renderer
    void R_Null_Init()
    {
        Logging_LogChannel("******** R_Null_Init ********", LogChannel::Debug);

        Logging_LogChannel("******** R_Null_Init was successful! ********", LogChannel::Message);
    }

    // Called on each frame to update the state of the wporld.
    void R_Null_Frame()
    {

    }

    // Shuts down the renderer
    void R_Null_Shutdown()
    {
        renderer_running = false; 
    }
}
