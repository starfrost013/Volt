#pragma once
#include <common/common.hpp>

//
// VOLT
// Console Subsystem
//

namespace Volt
{
    // Rendering type of the console.
    enum ConsoleType
    {
        // A full screen command line console
        CommandLineConsole = 0,

        // The console in the game
        GameConsole = 1,
    };

    extern ConsoleType console_type;

    void Console_Init(ConsoleType type);

    void Console_Frame();
    void Console_ExecuteCommand(const char* cmd);
    void Console_Shutdown();
}