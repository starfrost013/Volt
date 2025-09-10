#include <render/render.hpp>
#include <render/gl4/render_gl4.hpp>
#include <render/null/render_null.hpp>

//
// VOLT
// Copyright © 2025 starfrost
//
// render_texture.cpp: Texture implementation
//

namespace Volt
{
    template <Vector2i Size>
    Texture* Render_CreateTexture(TextureFormat format, const char* path)
    {
        Texture* texture = Memory_Alloc<Texture>(TAG_RENDER_TEXTURE);

        texture->size = Size;
        texture->format = format;

        texture->pixels = Memory_Alloc<uint32_t, Size>(TAG_RENDER_TEXTURE);

        // Slab allocate 

        //TODO: Vector2I?
        Logging_LogChannel("Render: Creating texture (size %.d,%.d)", LogChannel::Debug); 

        renderer_state_global.Texture_CreateFunction(texture);
    }
}