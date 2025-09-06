#include <render/null/render_null.hpp>

namespace Volt
{
    /* Globals */
    RendererStateNull render_state_null;

    // Initialises the GL4 renderer
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
        renderer_state_global.running = false; 
    }

    bool R_Null_CompileShader(VoltShaderSet* set, VoltShaderType type, VoltFile* file)
    {
        return true; 
    }

    bool R_Null_FreeShader(VoltShaderSet* set, VoltShaderType type, VoltFile* file)
    {
        return true; 
    }
}
