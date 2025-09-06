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
    bool R_Null_CompileShader(VoltShaderSet* set, VoltShaderType type, VoltFile* file);
    bool R_Null_FreeShader(VoltShaderSet* set, VoltShaderType type, VoltFile* file);

}
