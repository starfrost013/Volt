#pragma once
#include <common/common.hpp>

//
// command.hpp: Command Subsystem
// 23 February 2025
//

namespace Volt
{

    // Flags for the command - where did it come from
    // "Command" is to prevent conflict with client class
    enum CommandType
    {
        ClientCommand = 1,

        ConsoleCommand = (1 << 1),

        ServerCommand = (1 << 2),

        // A special value that indicates it can be used everywhere.
        GlobalCommand = ClientCommand | ConsoleCommand | ServerCommand,
    };

    // Defines a command.
    struct Command
    {
        const char* name;                           // The name of this command.
        CommandType type;                           // Determines where in the engine the command is allowed to come from. 
        void (*on_execute)(CommandType origin);     // Function to run when this command is executed.
        Command* prev;
        Command* next; 
    };
    
    extern Command* command_head;
    extern Command* command_tail;

    void Command_Init();
    Command* Command_Add(const char* name, CommandType type, void (*on_execute)(CommandType origin));       // Add a command to the command system.
    Command* Command_FindByName(const char* name);                                                          // Find a command by its name

    uint32_t Command_Argc();                                                                                // Get the parameter count of the last executed command.
    const char* Command_Argv(uint32_t argv);                                                                // Get a parameter of the last executed command.

    void Command_Execute(const char* cmd, CommandType origin);                                              // Execute a command string.
    void Command_ExecuteGlobal(const char* cmd);                                                            // Execute a command that can be executed from anywhere.
    
    void Command_Shutdown();                                                                                // Shut down the command system.

    // GLOBAL COMMAND CREATION FUNCTIONS
    
    void Command_CreateBasicCommands();

    // COMMANDS STORED IN DIFFERENT FILES
    void Command_Exec(CommandType origin);
}
