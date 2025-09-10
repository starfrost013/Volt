//
// VOLT
// Copyright © 2025 starfrost
//
// render_gl4.hpp: 
// Header file for the OpenGL 4.x renderer. This is a OpenGL 4.3/4.4/4.5/4.6 hardware-accel
//


#pragma once
#include <render/render.hpp>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

namespace Volt
{

    // Sets up the tasks the renderer can do
    enum RendererStateGL4Task
    {
        Spin = 0,

        Blit = 1,
    };

    // Holds the OpenGL 3.x renderer state.
    struct RendererStateGL4
    {
        RendererState global;

        GLFWwindow* window;
        GLFWmonitor* monitor_primary; 

        // used to remember the last window position when we go into fullscreen
        int32_t last_window_pos_x;
        int32_t last_window_pos_y;

        std::thread video_thread; 
        RendererStateGL4Task task;
    };

    extern RendererStateGL4 render_state_gl4;

    /* Functions */

    void R_GL4_Init();
    void R_GL4_Frame();
    void R_GL4_Shutdown();
    bool R_GL4_CompileShader(VoltShaderSet* set);
    void R_GL4_UseShader(VoltShaderSet* set);
    bool R_GL4_FreeShader(VoltShaderSet* set);
    void R_GL4_CreateTexture(Texture* texture);
}
