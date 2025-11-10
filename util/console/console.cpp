#include "util/util.hpp"

namespace Volt
{
	// various buffer size demands
	#define CONSOLE_ESC_CHAR				"\x1B"	//Some commands don't use it
	#define CONSOLE_TERMINAL_COMMAND_PREFIX "\x1B["
	#define CONSOLE_COLOR_BUFFER_SIZE		24		// maded a bit bigger for safety

	void Util_ConsoleSetForegroundColor(ConsoleColor color)
	{
		int32_t color_final = 30 + color;

		if (color >= ConsoleColor::FIRST_BRIGHT) 
			color_final = 90 + (color & ConsoleColor::FIRST_BRIGHT - 1);

		// 10 (max) + 1 + 4 + 1 for safety
		char final_string[CONSOLE_COLOR_BUFFER_SIZE] = { 0 };

		snprintf(final_string, CONSOLE_COLOR_BUFFER_SIZE, "%s%dm", CONSOLE_TERMINAL_COMMAND_PREFIX, color_final);

		printf("%s", final_string);
	}

	void Util_ConsoleSetBackgroundColor(ConsoleColor color)
	{
		int32_t color_final = 40 + color;

		if (color >= ConsoleColor::FIRST_BRIGHT) 
			color_final = 100 + (color & ConsoleColor::FIRST_BRIGHT - 1);

		// 10 (max) + 1 + 4 + 1 for safety
		char final_string[CONSOLE_COLOR_BUFFER_SIZE] = { 0 };

		snprintf(final_string, CONSOLE_COLOR_BUFFER_SIZE, "%s%dm", CONSOLE_TERMINAL_COMMAND_PREFIX, color_final);
		
		printf("%s", final_string);
	}

	void Util_ConsoleResetForegroundColor()
	{
		printf(CONSOLE_TERMINAL_COMMAND_PREFIX "39m");
	}

	void Util_ConsoleResetBackgroundColor()
	{
		printf(CONSOLE_TERMINAL_COMMAND_PREFIX "49m");
	}

	void Util_ConsoleClearScreen()
	{
		printf(CONSOLE_TERMINAL_COMMAND_PREFIX "2J"); // Clear screen.
		printf(CONSOLE_TERMINAL_COMMAND_PREFIX "3J"); // Clear scrollback.
		printf(CONSOLE_TERMINAL_COMMAND_PREFIX "0;0H"); // Move to 0,0.
	}

	// These are relative to the viewport
	void Util_ConsoleSetPosition(int32_t x, int32_t y)
	{
		printf(CONSOLE_TERMINAL_COMMAND_PREFIX "%d;%dH", y, x);	
	}

	// These are relative to the entire console buffer
	void Util_ConsoleSetPositionAbsolute(int32_t x, int32_t y)
	{
		printf(CONSOLE_TERMINAL_COMMAND_PREFIX "%dd", y);
		printf(CONSOLE_TERMINAL_COMMAND_PREFIX "%dG", y);
	}

	// Save the cursor position
	void Util_ConsoleSaveCursorPosition()
	{
		printf(CONSOLE_TERMINAL_COMMAND_PREFIX "7");
	}

	// Restore the cursor position
	void Util_ConsoleRestoreCursorPosition()	// Restore the cursor position
	{
		printf(CONSOLE_TERMINAL_COMMAND_PREFIX "8");
	}	
						
}
