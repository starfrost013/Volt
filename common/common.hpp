#pragma once
// common.hpp : Core Headers include for common component

// include common stuff

#include <Volt.hpp>
#include <common/cvar/cvar.hpp>             // CVar system
#include <common/memory/memory_alloc.hpp>   // Memory allocator
#include <common/net/net.hpp>               // Network system
#include <common/cmdline/cmdline.hpp>       // Command line parser
#include <common/command/command.hpp>       // Command system
#include <common/console/console.hpp>       // Console
#include <common/filesystem/filesystem.hpp> // Filesystem
#include <util/util.hpp>
#include <util/logging/logging.hpp>
#include <util/math/math.hpp>

namespace Volt
{
    #define ENGINE_TICKRATE		        60		// The engine's tickrate

    extern double max_time;                     // Time of the last frame
    extern double common_last_frametime;
    extern double common_last_updatetime;
    extern Cvar* common_tickrate;
    extern bool common_is_running;              // Determines if the app is running

    // Base component headers
    void Common_Init(int32_t argc, char** argv);
    void Common_Shutdown(uint32_t exit_code = 0);

    void Common_Main();
    void Common_Tick();
    void Common_Frame();

    // Called on fatal error
    void Common_Fatal();
}
