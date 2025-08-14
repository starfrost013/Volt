#include <common/common.hpp>
#include <sys/sys.hpp>

//
// cmdline.cpp
// Logic for the command line subsystem
// 

namespace Volt
{
	uint32_t cmdline_argc;					// Holds the number of the engine's command line options
	char** cmdline_argv;					// Holds a pointer to the engine's command line options

	/* Functions only used in this translation unit */
	void Cmdline_EarlyCreateCvars();

	void Cmdline_Init(int32_t argc, char** argv)
	{
		// is this safe on all platforms?
		cmdline_argv = argv;
		cmdline_argc = argc;
		Cmdline_EarlyCreateCvars();
	}
	
	// Check if a command line option exists.
	bool Cmdline_Check(const char* option)
	{
		for (uint32_t arg = 0; arg < cmdline_argc; arg++)
		{
			if (!strcasecmp(cmdline_argv[arg], option))
				return true;
		}
	
		return false; 
	}

	uint32_t Cmdline_Argc()
	{
		return cmdline_argc;
	}

	const char* Cmdline_Argv(uint32_t c)
	{
		if (c >= cmdline_argc
			|| c < 0)
		{
			Logging_LogChannel("Cmdline_Argv: Tried to obtain invalid command line option %d (range 0-%d). Returning empty string...", 
				LogChannel::Error, c, cmdline_argc); 
			return STRING_EMPTY;
		}

		return cmdline_argv[c];
	}

	/* See if any of our commands start with a +. If so, use that to create some cvars...*/
	void Cmdline_EarlyCreateCvars()
	{
		/* -2 because we need to provide a "+set", cvar name, and the value*/
		for (uint32_t arg = 0; arg < cmdline_argc - 2; arg++)
		{
			// don't crash
			if (!cmdline_argv[arg])
				break;

			if (!strcasecmp(cmdline_argv[arg], "+set"))
			{
				char* name = cmdline_argv[arg + 1];
				char* value = cmdline_argv[arg + 2];

				Logging_LogChannel("+set detected on command line creating cvar - %s, %s", LogChannel::Debug, name, value);
				Cvar_Set(name, value, false);
				arg += 2; // skip the ones we don't need
				continue;
			}
		}
	}
}