#include <render/null/render_null.hpp>

namespace Volt
{
    /* Globals */
    RendererStateNull render_state_null;

    // Initialises the GL4 renderer
    void R_Null_Init()
    {
        Logging_LogChannel("******** R_Null_Init ********", LogChannel::Debug);

        //setup functions
        renderer_state_global.Shader_CompileFunction = R_Null_CompileShader;
        renderer_state_global.Shader_UseFunction = R_Null_UseShader;
        renderer_state_global.Shader_FreeFunction = R_Null_FreeShader;

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

    bool R_Null_CompileShader(VoltShaderSet* set)
    {
        return true; 
    }

    void R_Null_UseShader(VoltShaderSet* set)
    {

    }

    bool R_Null_FreeShader(VoltShaderSet* set)
    {
        return true; 
    }
}
