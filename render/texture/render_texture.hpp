#include <render/render.hpp>

//
// VOLT
// Copyright © 2025 starfrost
//
// render_texture.cpp: Texture definition
//

namespace Volt
{
    struct Texture
    {
        uint32_t size_x;
        uint32_t size_y;

        uint32_t id;
    };

    Texture* Render_CreateTexture(const char* path);
}