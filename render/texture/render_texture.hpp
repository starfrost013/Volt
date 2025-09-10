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

        uint32_t id;
        
        TextureFormat format; 

        uint32_t* pixels; 
    };

    template <Vector2i Size>
    Texture* Render_CreateTexture(TextureFormat format, const char* path = nullptr); // path is optional
}