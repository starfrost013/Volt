#include <common/command/command.hpp>

//
// commands_basic.cpp
// Implements some basic/core commands for the command system
//

namespace Volt
{
    /* Functions only used in this translation unit */
    void Command_CvarCreate(CommandType origin);
    void Command_CvarSet(CommandType origin);
#ifdef DEBUG
    void Command_Echo(CommandType origin);
#endif

    void Command_CreateBasicCommands()
    {
        // create some basic commands
        Command_Add("create", CommandType::GlobalCommand, Command_CvarCreate);
        Command_Add("set", CommandType::GlobalCommand, Command_CvarSet);
        Command_Add("exec", CommandType::GlobalCommand, Command_Exec); // should this be limited in some way?

        // Debug commands
#ifdef DEBUG
        Command_Add("echo", CommandType::GlobalCommand, Command_Echo);
#endif
    }

    void Command_CvarCreate(CommandType origin)
    {
        auto flags = 0;

        if (Command_Argc() < 2)
        {
            Logging_LogChannel("Usage: create <name> <value> [flags]\nCreates a convar.", LogChannel::Message);
            return;
        }
        else if (Command_Argc() >= 3) // if flags were provided
            flags = atoi(Command_Argv(3));

        // remove any "internal" flags if it is a console command
        if (origin == CommandType::ConsoleCommand)
            flags &= ~(Cvar_NoUser);

        Cvar_Get(Command_Argv(1), Command_Argv(2), (CvarFlags)flags);
    }

    void Command_CvarSet(CommandType origin)
    {
        auto flags = 0;

        if (Command_Argc() < 2)
        {
            Logging_LogChannel("Usage: set <name> <value> [flags]\nSets the value of a convar.", LogChannel::Message);
            return;
        }
        else if (Command_Argc() >= 3) // if flags were provided
            flags = atoi(Command_Argv(3));

        // remove any "internal" flags if it is a console command
        if (origin == CommandType::ConsoleCommand)
        {
            flags &= ~(Cvar_NoUser);
            Cvar_Set(Command_Argv(1), Command_Argv(2), (CvarFlags)flags, false);
        }
        else
            Cvar_Set(Command_Argv(1), Command_Argv(2), (CvarFlags)flags, true);
    }

#ifdef DEBUG
    void Command_Echo(CommandType origin)
    {
        Logging_LogAll(Command_Argv(1));
    }
#endif
}