// 
// VOLT
// Copyright © 2024-2025 starfrost
//
// render_core.cpp: Renderer-agnostic part of the renderer
//

#include <render/render.hpp>
#include <render/gl4/render_gl4.hpp>
#include <render/null/render_null.hpp>

namespace Volt
{
    // Cvars
    Cvar* renderer;
    
    Cvar* render_width;
    Cvar* render_height;
    Cvar* render_fullscreen;
    Cvar* render_window_title;

    // Globals
    RendererState renderer_state_global;

    // We're going to have to break the rules for this... (786432 bytes)
    #define TEMP_SIZE_EMUTEXTURE            1024 * 768

    // Selects and initialises the renderer.
    void Render_Init()
    {
        // set the renderer type based on the "renderer" cvar

        renderer = Cvar_Get("renderer", "gl4", false);

        // set up some basic cvars 
        render_width = Cvar_Get("render_width", "1024", false);
        render_height = Cvar_Get("render_height", "768", false);
        render_fullscreen = Cvar_Get("render_fullscreen", "0", false);
        
        snprintf(renderer_state_global.window_title, WINDOW_TITLE_BUF_SIZE, "%s Window (%s @ %s)", APP_NAME, VERSION, BUILD_DATE);

        render_window_title = Cvar_Get("render_window_title", renderer_state_global.window_title, false);

        // because SOMEONE decided to run the engine with "+set render_width -1"
        // check for sane width and height values and set them
        if (render_width->value <= 0
            || render_width->value >= 10000)
            Cvar_Set("render_width", "1024", false);

        if (render_height->value <= 0
            || render_height->value >= 10000)
            Cvar_Set("render_height", "768", false);
        
        // set render_type and the rendere based on the "render" cvar
        if (!strcasecmp(renderer->string, "gl4"))
            renderer_state_global.type = RendererType::GL4;
        else if (!strcasecmp(renderer->string, "null"))
            renderer_state_global.type = RendererType::Null;

        Command_Add("loadshader", Command_LoadShader);
        
        // Init functions set up function pointers, there's no point using the function pointers
        switch (renderer_state_global.type)
        {
            case RendererType::GL4: // gl4 renderer
                Logging_LogChannel("Selected Renderer: OpenGL 4.3, Core Profile", LogChannel::Message);
                R_GL4_Init();
                break;
            case RendererType::Null: // Null debug renderer
                Logging_LogChannel("Selected Renderer: Null renderer", LogChannel::Message);
                R_Null_Init();
                break;
        }

        Vector2i size = Vector2i(1024, 768);

        Render_CreateTexture<TEMP_SIZE_EMUTEXTURE>(Volt::TextureFormat::RGBA32, size, "Generic");

        renderer_state_global.running = true; 
    }

    void Render_Frame()
    {
        renderer_state_global.FrameFunction();
    }

    // Shuts down the selected renderer.
    // TODO: Shutdown when render size changes
    void Render_Shutdown()
    {
        renderer_state_global.ShutdownFunction();

        renderer_state_global.running = false; 
    }
}