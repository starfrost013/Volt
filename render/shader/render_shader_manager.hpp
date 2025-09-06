// 
// VOLT
// Copyright © 2025 starfrost
//
// render_shader_manager.hpp: API-agnostic shader manager for the renderer
//

#pragma once
#include <common/common.hpp>

namespace Volt
{
    #define MAX_SHADER_NAME_LENGTH      128 // Doesn't need to be as long as MAX_STRING_LENGTH

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
        char* code;                     // It's the code of the shade; is this a good idea?
        char name[MAX_SHADER_NAME_LENGTH];
        char path[FS_MAX_PATH];         // A buffer holding the path
    };

    // Defines a set of shaders 
    struct VoltShaderSet
    {
        // These are the only ones that we should use 
        VoltShader vertex;
        VoltShader fragment;
        VoltShader compute;
        VoltShader geometry;
        
        VoltShaderSet* prev; 
        VoltShaderSet* next; 
    };

    extern VoltShaderSet* shader_set_head;
    extern VoltShaderSet* shader_set_tail;
    
    // THIS IS AWFUL
    extern std::vector<char> shader_tempbuf;

    bool Shader_LoadSet(const char* vertex = nullptr, const char* fragment = nullptr, const char* compute = nullptr, const char* geometry = nullptr);
    bool Shader_UnloadSet(VoltShaderSet* set);

    void Shader_Shutdown();
}