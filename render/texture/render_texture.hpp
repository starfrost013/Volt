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

        uint32_t id;                            // Texture id
        
        TextureFormat format; 

        uint32_t* pixels;

        // Todo: render-agnostic...
        uint32_t vertexBuffer;                  // Vertex Buffer ID
        uint32_t vertexArray;                   // Vertex Array ID
    };

    // Sorry for designing a memory allocator like this :(
    template <size_t SizeLinear>
    Texture* Render_CreateTexture(TextureFormat format, Vector2i size, const char* path = nullptr); // path is optional

    template <size_t SizeLinear>
    void Render_FreeTexture(Texture* texture); // path is optional
}