#include <common/console/console.hpp>

//
// console.cpp
// Implements the client subsystem
//

namespace Volt
{
    ConsoleType console_type = ConsoleType::CommandLineConsole;

    // increase this as you add more
    #define NUM_FUNNY_MESSAGES    21

    const char* funny_console_messages[] =
    {
        "Unity? More like Division!",
        "With framerates as unreal as Unreal's!",
        "nVIDIA RIVA 128 powered(tm)",
        "BUY DODCP",
        "Powered by 1000 hamster wheels running at centrifugal speed",
        "Python sucks",
        "Segmentation fault - core dumped",
        "Gerald Robotnik died for this",
        "Now exclusively for Xenix",
        "Step7.exe has stopped working. Windows won't check online for a solution to the problem. Fix it",
        "Bad Vibes Only",
        "ORIGINAL SOFTWARE DO NOT STEAL",
        "Big Balls approved",
        "exit::shikari();",
        "TOO EXTREM",
        "The one thing G.U.N. fears...",
        "No animals were harmed in the making of this game engine",
        "Brought to you by the Ultor Corporation",
        "The first modern game engine EXCLUSIVELY for the S3 ViRGE!",
        "Grand Theft Original Idea (G.T.O.I.)",
        "With pioneering new \"Ion\" technology, \"Electron\"s won't be bothering you anymore!",
    };

    // The current funny message number.
    uint32_t funny_message_id = 0;

    /* Functions only used in this translation unit */
    void Console_RenderCommandLine();
    void Console_RenderGame();

    void Console_Init(ConsoleType type)
    {
        console_type = type; 
        funny_message_id = rand() % NUM_FUNNY_MESSAGES;


    }

    void Console_Render()
    {

        switch (console_type)
        {
            case ConsoleType::CommandLineConsole:
                Console_RenderCommandLine();
                break;
            case ConsoleType::GameConsole:
                Console_RenderGame();
                break;
        }
    }

    void Console_RenderCommandLine()
    {
        Util_ConsoleSaveCursorPosition();
        Util_ConsoleSetPosition(0, 0);
        Util_ConsoleSetBackgroundColor(ConsoleColor::BrightBlue);
        Util_ConsoleSetForegroundColor(ConsoleColor::BrightWhite);
        printf(APP_NAME " " VERSION " " BUILD_DATE "\n");
        printf("%s\n", funny_console_messages[funny_message_id]);
        printf("Tickrate - %.2f; %.2f FPS (%.2fms last update + %.2fms last render. Max time for this tickrate = %.2fms)\n", 
            server_tickrate->value, 1000.0 / common_last_frametime, common_last_updatetime, common_last_frametime, max_time);
        
        // can't expose this to render code?
        Cvar* renderer = Cvar_Get("renderer", "gl3", false);
        printf("Using renderer: %s", renderer->string);

        Util_ConsoleResetBackgroundColor();
        Util_ConsoleResetForegroundColor();
        Util_ConsoleRestoreCursorPosition();
    }

    void Console_RenderGame()
    {

    }

    void Console_ExecuteCommand(const char* cmd)
    {
        Command_Execute(cmd, CommandType::ConsoleCommand);
    }

    void Console_Shutdown()
    {

    }
}