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
    template <size_t SizeLinear>
   

    void Render_DrawTexture(Texture* texture, Vector2 position, Vector2 scale)
    {
        VoltShaderSet* shader_set = Shader_GetByName(texture->shader_name);

        if (!shader_set)
            Logging_LogChannel("Tried to draw a texture with unloaded shader %s", LogChannel::Fatal, texture->shader_name);

        Shader_UseSet(shader_set);

        Matrix44 model_matrix = {0};

        Vector4 scale_factor = { scale.x, scale.y, 1.0f, 1.0f };

        model_matrix = model_matrix.Translate(Vector4(position.x, position.y, 0.0f, 0.0f));
        model_matrix = model_matrix.Scale(scale_factor);

        shader_set->SetMatrix4("model", model_matrix);

        renderer_state_global.Texture_DrawFunction(texture);
    }
}