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

    // Sets up the tasks the renderer can do
    enum RendererStateGL3Task
    {
        Spin = 0,

        Blit = 1,
    };

    // Holds the OpenGL 3.x renderer state.
    struct RendererStateGL3
    {
        RendererState global;

        GLFWwindow* window;
        GLFWmonitor* monitor_primary; 

        // used to remember the last window position when we go into fullscreen
        int32_t last_window_pos_x;
        int32_t last_window_pos_y;

        std::thread video_thread; 
        RendererStateGL3Task task;
    };

    extern RendererStateGL3 render_state_gl3;

    /* Functions */

    void R_GL3_Init();
    void R_GL3_Frame();
    void R_GL3_Shutdown();
}
