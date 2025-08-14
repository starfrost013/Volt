#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

//
// Logging system
// 
// Oct 2023: Wrote it
// Jan 2024: Fixed it...and added log_channel::Debug.
// Aug 2024: Fixed it even more, and also made it C++.
// Nov 2024: Fixed bright colours, simplified interface to use a singleton, other minor tweaks.
// Feb 2025: Correctly namespaced it, use C++ enum names.
// June 8, 2025		Use C++-style declarations, remove Logger_SetOptions and add fatal_function, a function pointer to call if a fatal log message occurs.
// 					Move from 5 strncat calls to one snprintf call
// June 28, 2025	Add superfatals for when memory corruption makes it not safe to continue
//

namespace Volt
{
	// Enumerates logging channels. This is a set of flags, so the user can enable anything they want.
	enum LogChannel
	{
		// Prints only on debug builds.
		Debug = 1,

		// Prints advisory messages.
		Message = 2,

		// Prints warnings. The user needs to know about them, but they don't impair program operation.
		Warning = 4,

		// Prints errors that impair program operation.
		Error = 8,

		// Prints fatal errors that require the program to exit.
		Fatal = 16,

		// Memory corruption detected. Even calling Common_Shutdown isn't safe. 
		SuperFatal = 32,

	};

	// Enumerates available log output sources.
	enum LogDestination
	{
		// Logs to standardout using vprintf.
		Printf = 1,

		// Logs to a file. (currently "latest.log")
		File = 2,
	};

	// Struct storing log settings.
	struct LogSettings
	{
		const char* file_name;
		LogChannel channels;
		LogDestination source;
		bool keep_old_logs;
		void (*fatal_function)();
	};

	// Struct storing the actual logger itelf.
	struct Logger
	{
		LogSettings settings;
		FILE* handle;
		bool initialised;
	};

	bool Logging_Init();
	void Logging_LogChannel(const char* text, LogChannel channel, ...);
	void Logging_LogAll(const char* text, ...);
	void Logging_Shutdown();

	// Logger object
	extern Logger logger;
}

