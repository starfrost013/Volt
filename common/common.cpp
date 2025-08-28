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
    Cvar* common_tickrate; 
    bool common_is_running;                 // Determines if common is actually active

    // Usually these would be the last framerate
    double max_time;                 // Time of the last update cycle
    double common_last_frametime;            // Time of the last frame.
    double common_last_updatetime;            // Time of the last update.

    std::thread* emu_thread;

    // Initialise cvars
    void Common_InitCvar()
    {
        common_tickrate = Cvar_Get("common_tickrate", "60", true);
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
        Common_InitCvar();                              // Initialise cvars
        Common_SignonMessage();                         // Print the sign-on message
        Filesystem_Init();                              // Initialise the filesystem
        Command_Init();                                 // Initialise the command system
        Cmdline_Init(argc, argv);                       // Initialise the command line
        Net_Init(NetworkType::Loopback);                // Initialise the network

        // Initialise emulation layer.
        Emulation_Init();

        common_is_running = true;
        
        //  Last step: Run autoexec.volt
        
        Console_ExecuteCommand("exec autoexec.volt");
        
        Logging_LogChannel("******** Common_Init done! ********", LogChannel::Message);
        
        // Start emulating
        Emulation_Start(); 

        // For now, clear all the backscroll
        //Util_ConsoleClearScreen();

        // start up the emulation thread
        // constructor can't be called by Memory_Alloc and this is a special case anyway
        emu_thread = new std::thread(Emulation_Tick);

        Common_Main();
    }

    /* Update function for the common component */
    void Common_Frame()
    {
        Console_Frame();
    }
    
    /* Tick function for the common component */
    void Common_Tick()
    {
    }

    void Common_Main()
    {
        // various measurements
        double real_milliseconds = 0, target_milliseconds = 0, render_milliseconds = 0, update_milliseconds = 0;
        double start = 0, frame_end = 0, end = 0;

        while (common_is_running)
        {
            
            // figure out how long we have to run
            start = Util_GetMicroseconds();

            // run common and emulation functions
            Common_Frame();

            frame_end = Util_GetMicroseconds();

            bool run_tick = real_milliseconds > target_milliseconds;
            
            Render_Frame();

            // Run non-emulation ticked stuff last
            if (run_tick)
                Common_Tick();

            end = Util_GetMicroseconds();

            /* we have to check a tick actually ran before we update update_milliseconds */
            update_milliseconds = double(end - frame_end) / 1000.0;

            render_milliseconds = double(frame_end - start) / 1000.0;

            if (real_milliseconds > target_milliseconds)
                real_milliseconds = double(end - start) / 1000.0;
            else   
                real_milliseconds += double(end - start) / 1000.0;
            
            target_milliseconds = 1000.0 * (1 / common_tickrate->value);

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

        Emulation_Shutdown(); 
        Render_Shutdown();

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
