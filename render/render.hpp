#pragma once
#include <common/common.hpp>
#include <render/display/render_display.hpp>
#include <render/texture/render_texture.hpp>
#include <render/shader/render_shader_manager.hpp>

// 
// VOLT
// Copyright © 2025 starfrost
//
// render.hpp: Shared header for files acorss all renderers
//

namespace Volt
{
    enum RendererType
    {
        // OpenGL 3.3
        GL4 = 0,

        // No renderer
        Null = 1,

        // The default renderer value.
        Default = GL4,
    };

    // Defines
    #define WINDOW_TITLE_BUF_SIZE       256

    // State shared across all renderers
    struct RendererState
    {
        char window_title[WINDOW_TITLE_BUF_SIZE];
        RendererType type; //default value does not matter
        bool running = false;
        bool (*Shader_CompileFunction)(VoltShaderSet* set);
        void (*Shader_UseFunction)(VoltShaderSet* set);
        bool (*Shader_FreeFunction)(VoltShaderSet* set);
        void (*Texture_CreateFunction)(Texture* texture);
        void (*Texture_LoadFromFile)(Texture* texture);
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
