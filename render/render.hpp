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

    // Defines
    #define WINDOW_TITLE_BUF_SIZE       256

    // State shared across all renderers
    struct RendererState
    {
        char window_title[WINDOW_TITLE_BUF_SIZE];
        RendererType type; //default value does not matter
        bool running = false;
    };

    extern RendererState renderer_state_global;


    // holds the global renderer type
    extern Cvar* renderer;

    extern Cvar* render_width;              // The width of the renderer.
    extern Cvar* render_height;             // The height of the renderer.
    extern Cvar* render_fullscreen;         // Determines if the renderer is in fullscreen (really borderless windowed)
    extern Cvar* render_window_title;       // The title of the window. Todo: Move to some kind of game name?

    void Render_Init();
    void Render_Frame();
    void Render_Shutdown();


}
