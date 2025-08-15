#include <common/common.hpp>
#include <emulation/emulation.hpp>
#include <render/render.hpp>
#include <sys/sys.hpp>

//
// common.cpp
// Code for managing the common subsystem.
//

namespace Volt
{
    // Holds the tickrate.
    Cvar* server_tickrate; 
    bool common_is_running;                 // Determines if common is actually active

    // Usually these would be the last framerate
    double max_time;                 // Time of the last update cycle
    double common_last_frametime;            // Time of the last frame.
    double common_last_updatetime;            // Time of the last update.

    // Initialise cvars
    void Common_InitCvar()
    {
        server_tickrate = Cvar_Get("server_tickrate", "60", true);
    }

    // Prints a sign-on message.
    void Common_SignonMessage()
    {
        Logging_LogChannel("" APP_NAME " " VERSION " " BUILD_DATE, LogChannel::Message);
        Logging_LogChannel("******** Common_Init starting ********", LogChannel::Debug);
    }

    void Common_Init(int32_t argc, char** argv)
    {
        logger.settings.fatal_function = Common_Fatal;  // Initialise logging
        Logging_Init();                                 
        Console_Init(ConsoleType::CommandLineConsole);  // Initialise the console
        Filesystem_Init();                              // Initialise the filesystem
        Common_InitCvar();                              // Initialise cvars
        Common_SignonMessage();                         // Print the sign-on message
        Cmdline_Init(argc, argv);                       // Initialise the command line
        Command_Init();                                 // Initialise the command system
        Net_Init(NetworkType::Loopback);                // Initialise the network

        // Initialise emulation layer.
        // Should this be a part of the code to the
        Emulation_Init();

        common_is_running = true;
        
       
        //  Last step: Run autoexec.s7cmd
        
        Console_ExecuteCommand("exec autoexec.s7cmd");
        
        Logging_LogChannel("******** Common_Init done! ********", LogChannel::Message);

        // For now, clear all the backscroll
        Util_ConsoleClearScreen();

        Common_Main();
    }

    /* Update function for the common component */
    void Common_Frame()
    {
        Console_Render();

        Emulation_Frame();
    }
    
    /* Tick function for the common component */
    void Common_Tick()
    {
        Emulation_Tick();
    }

    void Common_Main()
    {
        // various measurements
        double real_milliseconds = 0, target_milliseconds = 0, render_milliseconds = 0, update_milliseconds = 0;
        uint64_t start = 0, frame_end = 0, end = 0;

        while (common_is_running)
        {
            
            // figure out how long we have to run
            start = Util_GetMicroseconds();

            // run common and emulation functions
            Common_Frame();

            frame_end = Util_GetMicroseconds();

            // C++, WTF?
            // todo: replace this with SDL timers, QPC/getticks_ns or something 

            bool run_tick = real_milliseconds > target_milliseconds;

            // Update the world if it's time too
            if (run_tick)
            {
                Common_Tick();

                // ruun emulation tick 
            }

            end = Util_GetMicroseconds();

            /* we have to check a tick actually ran before we update update_milliseconds */
            if (run_tick)
                update_milliseconds = double(end - frame_end) / 1000.0;

            render_milliseconds = double(frame_end - start) / 1000.0;
            real_milliseconds = double(end - start) / 1000.0;
            target_milliseconds = 1000.0 * (1 / server_tickrate->value);

            // store the last full time and framerate
            max_time = target_milliseconds;
            common_last_frametime = render_milliseconds; 
            common_last_updatetime = update_milliseconds;
        }

        // now we're done so shut down
        Common_Shutdown();
    }
    
    void Common_Shutdown(uint32_t exit_code)
    {
        Logging_LogChannel("******** Common_Shutdown starting ********", LogChannel::Debug);
        // shut down the client, server and renderer if they are running

        //Emulation_Shutdown(); 
    
        // shut down in reverse order to starting up
        Net_Shutdown();
        Cvar_Shutdown();
        Command_Shutdown();
        Filesystem_Shutdown();
        Logging_LogChannel("******** Common_Shutdown done! *******", LogChannel::Message);
        Logging_Shutdown();

        /* now that we've fully shut down, we can just exit */
        exit(exit_code);
    }    

    void Common_Fatal()
    {
        Common_Shutdown(0x1);
    }
}
