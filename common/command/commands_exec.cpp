#include <common/command/command.hpp>

//
// commands_exec.cpp
// Implements the "exec" command. 
//

namespace Volt
{
    VoltFile* exec_cfg_ptr = nullptr;

    void Command_Exec(CommandType origin)
    {       
        if (Command_Argc() < 1)
        {
            Logging_LogChannel("Usage: exec <path to cfg file>", LogChannel::Message);
            return;
        }

        const char* file_path = Command_Argv(1);

        exec_cfg_ptr = Filesystem_OpenFile(file_path, Text, true);

        if (!exec_cfg_ptr)
        {
            Logging_LogChannel("exec: File not found!", LogChannel::Error);
            return;
        }

        VoltFile &exec_cfg = *exec_cfg_ptr; //so we can use streams

        while (!exec_cfg.eof)
        {
            Filesystem_ReadString(exec_cfg_ptr, fs_string_buf);

            Logging_LogChannel("exec file command: %s\n", LogChannel::Debug, fs_string_buf);
            Command_Execute(fs_string_buf, CommandType::ConsoleCommand); 
        }
    }
}
