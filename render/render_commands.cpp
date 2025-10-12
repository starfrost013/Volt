// 
// VOLT
// Copyright © 2025 starfrost
//
// render_commands.cpp: Exposes renderer interface to command engine
//

#include <common/common.hpp>

namespace Volt
{
    void Command_LoadShader()
    {
        uint32_t cmd_argc = Command_Argc();

        if (cmd_argc < 2)
        {
            Logging_LogChannel("At least a name, vertex and fragment path must be provided!"
                "Syntax: loadshader name [vertex_shader_path] [fragment_shader_path] [geometry_shader_path] [compute_shader_path] (At least one must be loaded!)"
                "Note: geometry and compute shaders don't do anything yet", LogChannel::Message);
            return; 
        }

        // used because command system returns a string ptr (for good reasons) but the default parameters for string functions are nullptr (also for good reasons)
        const char* name = Command_Argv(1);
        const char* vertex_path = Command_Argv(2);
        const char* fragment_path = Command_Argv(3);
        const char* geometry_path = Command_Argv(4);
        const char* compute_path = Command_Argv(5);

        const char* skip_string = "skip";

        if (vertex_path == STRING_EMPTY
        || strcasecmp(vertex_path, skip_string)) vertex_path = nullptr;
        if (fragment_path == STRING_EMPTY
        || strcasecmp(fragment_path, skip_string)) fragment_path = nullptr;
        if (geometry_path == STRING_EMPTY
        || strcasecmp(geometry_path, skip_string)) geometry_path = nullptr;
        if (compute_path == STRING_EMPTY
        || strcasecmp(geometry_path, skip_string)) compute_path = nullptr;

        // errors will be printed to the console so we don't need to check
        Shader_LoadSet(name, vertex_path, fragment_path, geometry_path, compute_path);     
    }
}