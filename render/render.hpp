#pragma once
#include <common/common.hpp>
#include <render/display/render_display.hpp>

// 
// VOLT
// Copyright © 2025 starfrost
//
// render.hpp: Shared header for files across all renderers
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
    #define WINDOW_TITLE_BUF_SIZE               256
    constexpr size_t SHADER_MAX_NAME_LENGTH =   64;

   // Used to compile the shader
    enum VoltShaderType
    {
        Vertex,
        Fragment,
        Compute,
        Geometry,
    };

    struct VoltShader
    {
        // This is the shader code
        char* code;                     // It's the code of the shader; is this a good idea? Only valid if !loaded
        bool loaded;                    // Is this valid?
        char path[FS_MAX_PATH];         // A buffer holding the path
        uint32_t id;                    // Backend shader ID
    };

    // Defines a set of shaders 
    struct VoltShaderSet
    {
        // These are the only ones that we should use 
        VoltShader vertex;
        VoltShader fragment;
        VoltShader compute;
        VoltShader geometry;
        
        uint32_t program_id;            // Backend program ID
        char name[SHADER_MAX_NAME_LENGTH];   // Shader name ID. Used to look up the shader (so we can load this info from files in the future)
        
        VoltShaderSet* prev; 
        VoltShaderSet* next; 
        
        void SetFloat(const char* name, float value);                   // Set shader parameter float
        void SetInt(const char* name, int32_t value);                   // Set shader parameter int32
        void SetVector2(const char* name, Vector2 value);               // Set shader parameter vec3
        void SetVector3(const char* name, Vector3 value);               // Set shader parameter vec3
        void SetVector4(const char* name, Vector4 value);               // Set shader parameter vec4
        void SetMatrix4(const char* name, Matrix44 value);              // Set shader parameter matrix4
    };

    // These are converted to real texture foramts using an std::unordered_map in each renderer
    enum TextureFormat
    {
        RGBA32 = 0, 
    };

    struct Texture
    {
        Vector2i size;

        uint32_t id;                                // Texture id
        
        TextureFormat format;                       // Format of the texture

        uint8_t* pixels;                            // sohuld be uint32_t so one unit == one texture
        char shader_name[SHADER_MAX_NAME_LENGTH];

        // Todo: render-agnostic...THIS IS VERY SLOW AND INEFFICIENT!
        uint32_t vertex_buffer;                     // Vertex Buffer ID
        uint32_t vertex_array;                      // Vertex Array
        uint32_t element_buffer;                    // Element Buffer 
    };

    // State shared across all renderers
    struct RendererState
    {
        char window_title[WINDOW_TITLE_BUF_SIZE];
        RendererType type; //default value does not matter
        bool running = false;
        bool (*Shader_CompileFunction)(VoltShaderSet* set);
        void (*Shader_UseFunction)(VoltShaderSet* set);
        bool (*Shader_FreeFunction)(VoltShaderSet* set);
        void (*Shader_SetFloat)(VoltShaderSet* set, const char* name, float value);
        void (*Shader_SetInt)(VoltShaderSet* set, const char* name, int32_t value);
        void (*Shader_SetVector2)(VoltShaderSet* set, const char* name, Vector2 value);
        void (*Shader_SetVector3)(VoltShaderSet* set, const char* name, Vector3 value);
        void (*Shader_SetVector4)(VoltShaderSet* set, const char* name, Vector4 value);
        void (*Shader_SetMatrix4)(VoltShaderSet* set, const char* name, Matrix44 value);
        void (*Texture_Create)(Texture* texture);
        void (*Texture_Draw)(Texture* texture);
        void (*Texture_Free)(Texture* texture);
        void (*Render_FrameStart)();
        void (*Render_FrameEnd)();
        void (*Render_Shutdown)();     
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

    extern VoltShaderSet* shader_set_head;
    extern VoltShaderSet* shader_set_tail;
    
    bool Shader_LoadSet(const char* use_shader_name, const char* vertex = nullptr, const char* fragment = nullptr, const char* compute = nullptr, const char* geometry = nullptr);
    bool Shader_UnloadSet(VoltShaderSet* set);
    void Shader_UseSet(VoltShaderSet* set);
    VoltShaderSet* Shader_GetByName(const char* use_shader_name);
    
    void Shader_Shutdown();

    // Sorry for designing a memory allocator like this :(
    template <size_t SizeLinear>
    Texture* Render_CreateTexture(TextureFormat format, Vector2i size, const char* use_shader_name, const char* path = nullptr) // size is optional
    {
        VoltShaderSet* shader = Shader_GetByName(use_shader_name);

        if (!shader)
            Logging_LogChannel("Tried to draw a texture with unloaded shader %s", LogChannel::Fatal, use_shader_name);

        Texture* texture = Memory_Alloc<Texture>(TAG_RENDER_TEXTURE);

        texture->size = size;
        texture->format = format;
        strncpy(texture->shader_name, use_shader_name, SHADER_MAX_NAME_LENGTH);
        // Slab allocate 
        texture->pixels = Memory_Alloc<uint8_t, SizeLinear>(TAG_RENDER_TEXTURE);

        // Version 0.000000000000000000000000000000001 ultra temporary code

        // should generate random pixels incl. random alpha so some will not show up
        for (size_t i = 0; i < SizeLinear; i++)
            texture->pixels[i] = rand() % 255;
            
        //TODO: Vector2I?
        Logging_LogChannel("Render: Creating texture (size %.d,%.d)", LogChannel::Debug, size.x, size.y); 

        renderer_state_global.Texture_Create(texture);

        return texture; 
    }

    void Render_DrawTexture(Texture* texture, Vector2 position, Vector2 scale);
    
    template <size_t SizeLinear>
    void Render_FreeTexture(Texture* texture)
    {
        if (!texture)
            Logging_LogChannel("Tried to call Render_FreeTexture on nullptr!", LogChannel::Fatal); // does not return

        renderer_state_global.Texture_Free(texture); 
    }

    // COMMANDS always at the end
    void Command_LoadShader(CommandType origin);
}
