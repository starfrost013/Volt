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
    Texture* Render_CreateTexture(TextureFormat format, Vector2i size, const char* use_shader_name, const char* path)
    {
        // Sorry for horrible design
        static_assert(SizeLinear == (size.x * size.y), "Incorrect texture size allocation!");

        Texture* texture = Memory_Alloc<Texture>(TAG_RENDER_TEXTURE);

        texture->size = size;
        texture->format = format;
        strncpy(texture->shader_name, use_shader_name, SHADER_MAX_NAME_LENGTH);
        // Slab allocate 
        texture->pixels = Memory_Alloc<uint32_t, SizeLinear>(TAG_RENDER_TEXTURE);

        //TODO: Vector2I?
        Logging_LogChannel("Render: Creating texture (size %.d,%.d)", LogChannel::Debug, size.x, size.y); 

        renderer_state_global.Texture_CreateFunction(texture);

        return texture; 
    }

    void Render_DrawTexture(Texture* texture)
    {
        VoltShaderSet* shader_set = Shader_GetByName(texture->shader_name);

        if (!shader_set)
            Logging_LogChannel("Tried to draw a texture with unloaded shader %s", LogChannel::Fatal, texture->shader_name);

        Shader_UseSet(shader_set);

        glGenVertexArrays(1, &texture->vertex_array);
        glGenBuffers(1, &texture->vertex_buffer);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->id);
        

    }

    template <size_t SizeLinear>
    void Render_FreeTexture(Texture* texture)
    {
        if (!texture)
            Logging_LogChannel("Tried to call Render_FreeTexture on nullptr!", LogChannel::Fatal); // does not return

        static_assert(SizeLinear == (texture->size.x * texture->size.y), "Incorrect texture size allocation!");

        renderer_state_global.Texture_FreeFunction(texture);    
    }
}