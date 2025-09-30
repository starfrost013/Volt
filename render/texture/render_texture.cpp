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
    template <size_t SizeLinear>
    Texture* Render_CreateTexture(TextureFormat format, Vector2i size, const char* path)
    {
        // Sorry for horrible design
        static_assert(SizeLinear == (size.x * size.y), "Incorrect texture size allocation!");

        Texture* texture = Memory_Alloc<Texture>(TAG_RENDER_TEXTURE);

        texture->size = size;
        texture->format = format;

        // Slab allocate 
        texture->pixels = Memory_Alloc<uint32_t, SizeLinear>(TAG_RENDER_TEXTURE);

        //TODO: Vector2I?
        Logging_LogChannel("Render: Creating texture (size %.d,%.d)", LogChannel::Debug, size.x, size.y); 

        renderer_state_global.Texture_CreateFunction(texture);

        return texture; 
    }

    template <size_t SizeLinear>
    void Render_FreeTexture(Texture* texture)
    {
        renderer_state_global.Texture_FreeFunction(texture);    
    }
}