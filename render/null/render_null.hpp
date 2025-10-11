#pragma once
#include <render/render.hpp>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

//
// render_null.hpp
// Header file for the Null renderer
//

namespace Volt
{
    // Holds the OpenGL 3.x renderer state.
    struct RendererStateNull
    {

        //deliberately blank
    };

    extern RendererStateNull render_state_null;

    /* Functions */

    void R_Null_Init();
    void R_Null_Frame();
    void R_Null_Shutdown();
    bool R_Null_CompileShader(VoltShaderSet* set);
    void R_Null_UseShader(VoltShaderSet* set);
    bool R_Null_FreeShader(VoltShaderSet* set);

    void R_Null_CreateTexture(Texture* texture);
    void R_Null_DrawTexture(Texture* texture);

    void R_Null_FreeTexture(Texture* texture);

    void R_Null_ShaderSetFloat(VoltShaderSet* set, const char* name, float value);
    void R_Null_ShaderSetInt(VoltShaderSet* set, const char* name, int32_t value);
    void R_Null_ShaderSetVector2(VoltShaderSet* set, const char* name, Vector2 value);
    void R_Null_ShaderSetVector3(VoltShaderSet* set, const char* name, Vector3 value);
    void R_Null_ShaderSetVector4(VoltShaderSet* set, const char* name, Vector4 value);
    void R_Null_ShaderSetMatrix4(VoltShaderSet* set, const char* name, Matrix44 value);
}
