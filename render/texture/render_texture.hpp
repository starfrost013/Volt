#pragma once
#include <render/render.hpp>

//
// VOLT
// Copyright © 2025 starfrost
//
// render_texture.cpp: Texture definition
//

namespace Volt
{
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

        uint32_t* pixels;
        char shader_name[SHADER_MAX_NAME_LENGTH];

        // Todo: render-agnostic...
        uint32_t vertex_buffer;                     // Vertex Buffer ID
        uint32_t vertex_array;                      // Vertex Array ID
    };

    // Sorry for designing a memory allocator like this :(
    template <size_t SizeLinear>
    Texture* Render_CreateTexture(TextureFormat format, Vector2i size, const char* use_shader_name, const char* path = nullptr); // path is optional

    void Render_DrawTexture(Texture* texture, Vector2 position, Vector2 scale);
    
    template <size_t SizeLinear>
    void Render_FreeTexture(Texture* texture); // path is optional
}