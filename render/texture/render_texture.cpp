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
    // TODO: File loading
    void Render_DrawTexture(Texture* texture, Vector2 position, Vector2 scale)
    {
        VoltShaderSet* shader_set = Shader_GetByName(texture->shader_name);

        if (!shader_set)
            Logging_LogChannel("Tried to draw a texture with unloaded shader %s", LogChannel::Fatal, texture->shader_name);

        Shader_UseSet(shader_set);

        renderer_state_global.Texture_DrawFunction(texture);
    }
}