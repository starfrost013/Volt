#include <common/command/command.hpp>

//
// command.cpp
// Implements the command system
// 

namespace Volt
{

    // Globals
    Command* command_head = nullptr;
    Command* command_tail = nullptr;

    /* This is probably not threadsafe */
    char last_command[MAX_STRING_LENGTH] = {0};
    /* This is so we can do stuff like strtok */
    char last_token[MAX_STRING_LENGTH] = {0};


    void Command_Init()
    {
        Logging_LogChannel("******** Command_Init ********", LogChannel::Debug);

        Command_CreateBasicCommands();
    }


    /* Adds a command. */
    Command* Command_Add(const char* name, CommandType type, void (*on_execute)(CommandType origin))
    {
        if (!name || strlen(name) <= 0)
        {
            Logging_LogChannel("Tried to add an invalid command name", LogChannel::Error);
            return nullptr; 
        }

        Command* command = Memory_Alloc<Command>(TAG_COMMAND);

        // if there are no commands, add some
        if (!command_head)
            command_head = command_tail = command;
        else
        {
            // do we need this list to be doubly linked?
            command->prev = command_tail;
            command_tail->next = command;
            command_tail = command;
        }

        command->name = name;
        command->type = type;
        command->on_execute = on_execute;

        Logging_LogChannel("Creating command %s", LogChannel::Debug, command->name);

        return command;
    }

    // Finds a command by its name
    Command* Command_FindByName(const char* name)
    {
        Command* current_command = command_head;

        while (current_command)
        {
            if (!strcasecmp(current_command->name, name))
                return current_command;
                
            current_command = current_command->next;
        }

        return nullptr;
    }

    uint32_t Command_Argc()
    {
        // restore the original
        strncpy(last_token, last_command, MAX_STRING_LENGTH);

        char* cmd_name = strtok(last_token, STRING_WHITESPACE_DELIMITERS);

        uint32_t amount = 0;

        // skip the command name

        char* tok = strtok(NULL, STRING_WHITESPACE_DELIMITERS);

        while (tok)
        {
            amount++;
            tok = strtok(NULL, STRING_WHITESPACE_DELIMITERS);
        }

        return amount; 
    }

    // Return all the text after the command name for the last command executed
    const char* Command_AllTextAfterName()
    {
        // restore the original
        strncpy(last_token, last_command, MAX_STRING_LENGTH);
        char* cmd_name = strtok(last_token, STRING_WHITESPACE_DELIMITERS);
        // skip the command name

        uint32_t name_length = strlen(cmd_name);

        if (name_length >= MAX_STRING_LENGTH)
            return last_token; 

        char* tok = last_token + name_length + 1;
        return tok; 
    }

    // Return parameter "argv" of command "command".
    const char* Command_Argv(uint32_t argv)
    {
        // restore the original
        strncpy(last_token, last_command, MAX_STRING_LENGTH);

        char* str = Util_GetTokenSeparatedPart(last_token, STRING_SINGLE_SPACE, argv);
        
        if (!str)
            return STRING_EMPTY;
    
        return str; 
    }

    void Command_Execute(const char* cmd, CommandType origin)
    {
        auto len = strlen(cmd);

        if (len > MAX_STRING_LENGTH)
            Logging_LogChannel("Command_Execute: Command string above %d characters. The command will be executed, but you won't be able to access parameters after the cutoff",
            LogChannel::Warning, MAX_STRING_LENGTH);

        strncpy(last_command, cmd, MAX_STRING_LENGTH);
        strncpy(last_token, cmd, MAX_STRING_LENGTH);

        char* cmd_name = strtok(last_token, STRING_WHITESPACE_DELIMITERS);

        Command* command_ptr = Command_FindByName(cmd_name);

        if (!command_ptr)
        {
            Logging_LogChannel("Command not found: %s", LogChannel::Warning, cmd_name);
            return;
        }

        if (!command_ptr->on_execute)
        {
            Logging_LogChannel("Command has no on-execute function: %s", LogChannel::Error, cmd_name);
            return;
        }

        if (command_ptr->type & origin)
            command_ptr->on_execute(origin);
    }

    /* Executes global type commands*/
    void Command_ExecuteGlobal(const char* cmd)
    {
        Command_Execute(cmd, CommandType::GlobalCommand);
    }

    /* Shuts down the command system*/
    void Command_Shutdown()
    {
        // nothing to do
        if (!command_head || !command_tail)
            return; 

        // free everything in the list.
        Command* command = command_head;

        while (command)
        {
            Command* next_free = command->next;
            Memory_Free(command);
            command = next_free;
        }

        command_head = command_tail = NULL;
    }
}