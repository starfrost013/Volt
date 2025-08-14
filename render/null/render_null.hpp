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
    struct NullRendererState
    {
        //deliberately blank
    };

    extern NullRendererState null_render_state;

    /* Functions */

    void R_Null_Init();
    void R_Null_Frame();
    void R_Null_Shutdown();
}
