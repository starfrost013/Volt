#pragma once
#include <render/render.hpp>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

//
// render_gl3.hpp
// Header file for the OpenGL 3.x renderer
//

namespace Volt
{
    // Holds the OpenGL 3.x renderer state.
    struct GL3RendererState
    {
        GLFWwindow* window;
        GLFWmonitor* monitor_primary; 

        // used to remember the last window position when we go into fullscreen
        int32_t last_window_pos_x;
        int32_t last_window_pos_y;
    };

    extern GL3RendererState gl3_render_state;

    /* Functions */

    void R_GL3_Init();
    void R_GL3_Frame();
    void R_GL3_Shutdown();
}
