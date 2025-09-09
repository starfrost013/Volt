// 
// VOLT
// Copyright © 2025 starfrost
//
// render_shader_manager.cpp: Core shader loader/unloader
//


#include "render_shader_manager.hpp"
#include <render/render.hpp>

namespace Volt
{
    #define STR_TEMPBUF_MIN_SIZE        1024

    VoltShaderSet* shader_set_head;
    VoltShaderSet* shader_set_tail;
    
    // Using a vector is the most sensible thing here.
    // This does not fit
    // Minimum size provided so that we don't endlessly resize ofr the first bit
    std::vector<char> shader_tempbuf(STR_TEMPBUF_MIN_SIZE);

    bool Shader_Load(const char* file_path, VoltShaderSet* shader_set, VoltShaderType shader_type)
    {
        // copy in the name of the file
        VoltShader &target = shader_set->vertex;

        // if its a vertex shader just use the value above
        if (shader_type == VoltShaderType::Fragment)
            target = shader_set->fragment;
        else if (shader_type == VoltShaderType::Geometry)
            target = shader_set->geometry;
        else if (shader_type == VoltShaderType::Compute)
            target = shader_set->compute;

        //probably will be removed anyway
        strncpy(target.path, file_path, FS_MAX_PATH);

        VoltFile* file = Filesystem_OpenFile(file_path, VoltFileMode::Text);

        if (!file)
        {
            Logging_LogChannel("Failed to open shader file %s", LogChannel::Error);
            return false; 
        }

        size_t size = Filesystem_GetFileSize(file);

        const char* shader_code; 

        // TODO: WARNING: CRAP!
        std::stringstream string;
        string << file->stream.rdbuf();
        target.code = string.str().c_str();

        // Pass the file to the render backend in case some backend-specific processing is needed on the file data
        if (!renderer_state_global.Shader_CompileFunction(shader_set))
            return false;

        // close it, we don't need the file anymore
        Filesystem_CloseFile(file); 

        // create the shader. Is this even needed?
                return true;
    }

    bool Shader_LoadSet(const char* vertex_path, const char* fragment_path, 
        const char* compute_path, const char* geometry_path)
    {
        if (!vertex_path
        && !fragment_path
        && !compute_path
        && !geometry_path)
        {
            Logging_LogChannel("Shader_LoadSet: You must load at least one of vertex, fragment, compute, and geometry shader!", LogChannel::Error);
            return false;
        }

        VoltShaderSet* shader_set = Memory_Alloc<VoltShaderSet>(TAG_RENDER_SHADER);

        // Figure out the size of the file

        // Try and compile the shaders
        if (vertex_path)
        {
            if (!Shader_Load(vertex_path, shader_set, VoltShaderType::Vertex))
            {
                Logging_LogChannel("Failed to compile vertex shader at %s!", LogChannel::Debug);
                return false;
            }
        }
        
        if (fragment_path)
        {
            if (!Shader_Load(fragment_path, shader_set, VoltShaderType::Fragment))
            {
                Logging_LogChannel("Failed to compile fragment shader at %s!", LogChannel::Debug);
                return false;
            }
        }
        
        if (compute_path)
        {
            if (!Shader_Load(compute_path, shader_set, VoltShaderType::Compute))
            {
                Logging_LogChannel("Failed to compile compute shader at %s!", LogChannel::Debug);
                return false;
            }
        }

        if (geometry_path)
        {
            if (!Shader_Load(compute_path, shader_set, VoltShaderType::Geometry))
            {
                Logging_LogChannel("Failed to compile geometry shader at %s!", LogChannel::Debug);
                return false;
            }
        }

        // put the file in the shader set chain
        if (!shader_set_head && !shader_set_tail)
        {
            shader_set_head = shader_set_tail = shader_set;
        }
        else
        {
            shader_set_tail->next = shader_set;
            shader_set_tail = shader_set;
            shader_set->prev = shader_set_tail;
        }

        return true; 
    }

    bool Shader_UnloadSet(VoltShaderSet* set)
    {
        if (set == shader_set_head)
        {
            if (set == shader_set_tail)
                shader_set_head = shader_set_tail = nullptr;
            else
            {
                shader_set_head = set->next;
            }
        }        
        else if (set == shader_set_tail)
        {
            // we already checked for both
            shader_set_tail = set->prev;
        }
        else
        {
            set->next->prev = set->prev;
            set->prev->next = set->next;
        }

        Memory_Free<VoltShaderSet>(set);

        return true; 
    }

    void Shader_UseSet(VoltShaderSet* set)
    {
        renderer_state_global.Shader_UseFunction(set);
    }

    void Shader_Shutdown()
    {

    }
}