#include "logging.hpp"
#include <util/util.hpp>

namespace Volt
{
	// Logging.cpp
	// Handles logging for the Step7 project

	// Defines 
	#define	LOGGING_MAX_LENGTH_TEXT		1024												// Maximum length of text being logged via the logger.
	#define LOGGING_MAX_LENGTH_DATE		32													// Length of the string containing the current date.
	#define LOGGING_MAX_LENGTH_TOTAL	LOGGING_MAX_LENGTH_TEXT + LOGGING_MAX_LENGTH_DATE	// Total length of the logging text buffer.

	#define ITS_OVER					0x43524150											// 'CRAP'

	// Prototypes for "internal" functions only used in this source file
	void Logging_Log(const char* text, LogChannel channel, va_list args);

	// not threadsafe :O
	Logger logger = { 0 };

	// Implementation
	bool Logging_Init()
	{
		//todo: allow this to be configured in settings

		if (!logger.settings.file_name) 
			logger.settings.file_name = "latest.log";
			
		logger.settings.channels = (LogChannel)(LogChannel::Debug | LogChannel::Message | LogChannel::Warning | LogChannel::Error | LogChannel::Fatal | LogChannel::SuperFatal);
		logger.settings.source = (LogDestination)(LogDestination::Printf | LogDestination::File);
		logger.settings.keep_old_logs = false; 

		if (logger.settings.source & LogDestination::File)
		{
			logger.handle = fopen(logger.settings.file_name, "w+");

			if (!logger.handle)
			{
				printf("Log failed: 0x0002DEAD Error opening logfile %s: errno %d\n", logger.settings.file_name, errno);
				return false;
			}
		}

		logger.initialised = true;

		return true;
	}

	void Logging_Shutdown()
	{
		fclose(logger.handle);
	}

	void Logging_LogChannel(const char* text, LogChannel channel, ...)
	{
		va_list	args;

		va_start(args, channel);

		Logging_Log(text, channel, args);
	}

	// Logs to all channels except the Fatal Error log channel.
	void Logging_LogAll(const char* text, ...)
	{
		va_list args;

		va_start(args, text);

		Logging_Log(text, (LogChannel)(LogChannel::Debug | LogChannel::Message | LogChannel::Warning | LogChannel::Error), args);
	}

	// yes this is required
	void Logging_Log(const char* text, LogChannel channel, va_list args)
	{
		// don't print messages on closed log channels
		if (!(logger.settings.channels & channel))
			return;

	// if solely printing debug channel, return on release build
	#ifdef RELEASE
		if (channel == LogChannel::Debug)
			return;
	#endif

		char date_buffer[LOGGING_MAX_LENGTH_DATE] = {0};
		char log_string_buffer[LOGGING_MAX_LENGTH_TOTAL] = {0};

		memset(date_buffer, 0x00, sizeof(date_buffer));
		memset(log_string_buffer, 0x00, sizeof(log_string_buffer));

		Util_DateGetCurrentString(date_buffer);

		size_t date_buffer_length = strlen(date_buffer);

		// avoids a crash by trying to decrement 1 from this value...whoops
		if (date_buffer_length <= 0)
			return;

		if (strlen(text) > LOGGING_MAX_LENGTH_TEXT)
		{
			printf("Log failed: cannot log string of length 0 or above %d bytes!", LOGGING_MAX_LENGTH_TEXT);
			return;
		}

		// lop off the last character so it doesn't have a new line

		date_buffer[date_buffer_length - 1] = '\0';

		const char* prefix = "[";
		const char* suffix_date = "]: ";
		const char* suffix = " \n";

		// print separate colours
		switch (channel)
		{
			case LogChannel::Message:	//shutup gcc
				break;
			case LogChannel::Debug:
				prefix = "[DEBUG] [";
				break;		
			case LogChannel::Warning:
				prefix = "[WARNING] [";
				break;
			case LogChannel::Error:
				prefix = "[ERROR] [";
				break;
			case LogChannel::Fatal:
				prefix = "[FATAL] [";
				break;
			case LogChannel::SuperFatal:
				prefix = "[SUPER FATAL] [";
				break;
		}

		assert(date_buffer);

		snprintf(log_string_buffer, sizeof(log_string_buffer), 
		"%s%s%s %s %s", prefix, date_buffer, suffix_date, text, suffix);

		if (logger.settings.source & LogDestination::Printf)
		{
			switch (channel)
			{
				case LogChannel::Message: //shutup GCC
					break;
				case LogChannel::Debug:
					Util_ConsoleSetForegroundColor(ConsoleColor::Blue);
					break;
				case LogChannel::Warning:
					Util_ConsoleSetForegroundColor(ConsoleColor::BrightYellow);
					break;
				case LogChannel::Error:
				case LogChannel::Fatal:
				case LogChannel::SuperFatal:
					Util_ConsoleSetForegroundColor(ConsoleColor::BrightRed);
					break;
			}

			vprintf(log_string_buffer, args);

			Util_ConsoleResetForegroundColor();

			// If a fatal error message is logged, we're going down,
			// so call the fatal error function if it is safe to do so
			if (channel & LogChannel::Fatal
				&& !(channel & LogChannel::SuperFatal)
			&& logger.settings.fatal_function)
			{
				logger.settings.fatal_function();
			}
				
		}

		if (logger.settings.source & LogDestination::File)
			vfprintf(logger.handle, log_string_buffer, args);

		if (channel & LogChannel::SuperFatal)
		{
			printf("MEMORY COMPLETELY HOSED. NOT SAFE TO CLEANLY SHUT DOWN - LET'S GET THE HELL OUT OF HERE!!!\n");
			exit(ITS_OVER);
		}

		va_end(args);
	}
}
