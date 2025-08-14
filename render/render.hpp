#pragma once
#include <common/common.hpp>

//
// render.hpp
// Header file for components shared across all renderers.
//

namespace Volt
{
    enum RendererType
    {
        // OpenGL 3.3
        GL3 = 0,

        // No renderer
        Null = 1,

        // The default renderer value.
        Default = GL3,
    };

    // holds the global renderer type
    extern Cvar* renderer;

    extern Cvar* render_width;              // The width of the renderer.
    extern Cvar* render_height;             // The height of the renderer.
    extern Cvar* render_fullscreen;         // Determines if the renderer is in fullscreen (really borderless windowed)
    extern Cvar* render_window_title;       // The title of the window. Todo: Move to some kind of game name?

    // Determines if the renderer is *actually* running or not
    extern bool renderer_running;

    void Render_Init();
    void Render_Frame();
    void Render_Shutdown();


}
