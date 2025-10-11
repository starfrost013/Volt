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
        renderer_state_global.Shader_SetInt = R_Null_ShaderSetInt;
        renderer_state_global.Shader_SetFloat = R_Null_ShaderSetFloat;
        renderer_state_global.Shader_SetVector2 = R_Null_ShaderSetVector2;
        renderer_state_global.Shader_SetVector3 = R_Null_ShaderSetVector3;
        renderer_state_global.Shader_SetVector4 = R_Null_ShaderSetVector4;
        renderer_state_global.Shader_SetMatrix4 = R_Null_ShaderSetMatrix4;

        renderer_state_global.Texture_CreateFunction = R_Null_CreateTexture;
        renderer_state_global.Texture_DrawFunction = R_Null_DrawTexture;
        renderer_state_global.Texture_FreeFunction = R_Null_FreeTexture;
        renderer_state_global.FrameFunction = R_Null_Frame;
        renderer_state_global.ShutdownFunction = R_Null_Shutdown;

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

    void R_Null_ShaderSetFloat(VoltShaderSet* set, const char* name, float value)
    {

    }

    void R_Null_ShaderSetInt(VoltShaderSet* set, const char* name, int32_t value)
    {

    }

    void R_Null_ShaderSetVector2(VoltShaderSet* set, const char* name, Vector2 value)
    {

    }

    void R_Null_ShaderSetVector3(VoltShaderSet* set, const char* name, Vector3 value)
    {

    }

    void R_Null_ShaderSetVector4(VoltShaderSet* set, const char* name, Vector4 value)
    {

    }

    void R_Null_ShaderSetMatrix4(VoltShaderSet* set, const char* name, Matrix44 value)
    {

    }

    void R_Null_CreateTexture(Texture* texture)
    {

    }

    void R_Null_DrawTexture(Texture* texture)
    {
        
    }
    
    void R_Null_FreeTexture(Texture* texture)
    {

    }
}
