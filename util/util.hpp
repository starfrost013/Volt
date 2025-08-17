#pragma once
#include <cassert>
#include <cmath>
#include <cstdarg>
#include <cstdbool>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cstring>

namespace Volt
{
	//
	//	Util.h: Implements "general" util functions.
	//

	// Global empty string literal to prevent typos, and as a default value to return.
	// DO NOT WRITE
	extern const char* STRING_EMPTY;
	extern const char* STRING_SINGLE_SPACE;
	extern const char* STRING_WHITESPACE_DELIMITERS;
	
	#define PARITY_TABLE_SIZE 	256
	
	extern const uint8_t byte_parity_table[PARITY_TABLE_SIZE];

	// Date utilities
	void Util_DateGetCurrentString(char *dateStr);								// Gets the current date and returns it as a C time format string.

	// String utilities
	bool Util_BooleanFromString(char* string);									// Converts a boolean from a string.
	char* Util_GetTokenSeparatedPart(char* fmt, const char* delim, uint32_t n);	// Get part n of a string buf split by token token.

	// Weird shim for MSVC vs. Posix...
#ifdef WIN32 
	#ifdef _MSC_VER
		#pragma warning(disable: 4996)
		#define strcasecmp stricmp
	#endif
#endif

	// Console utilities
	enum ConsoleColor
	{
		Black = 0,
		Red = 1,
		Green = 2,
		Yellow = 3,
		Blue = 4,
		Magenta = 5,
		Cyan = 6,
		White = 7,
		// this is just used to indicate the first "bright" console colour so other code is easier to read
		FIRST_BRIGHT = 8,
		BrightBlack = 8,
		BrightRed = 9,
		BrightGreen = 10,
		BrightYellow = 11,
		BrightBlue = 12,
		BrightMagenta = 13,
		BrightCyan = 14,
		BrightWhite = 15,
	};

	void Util_ConsoleSetPosition(int32_t x, int32_t y);				// Set the cursor position to line x,y within the current viewport.
	void Util_ConsoleSetPositionAbsolute(int32_t x, int32_t y);		// Set the cursor position to line x,y within the console buffer.
	void Util_ConsoleSaveCursorPosition();							// Save the cursor position
	void Util_ConsoleRestoreCursorPosition();						// Restore the cursor position
	void Util_ConsoleSetForegroundColor(ConsoleColor color);		// Set the foreground color of the console.
	void Util_ConsoleSetBackgroundColor(ConsoleColor color);		// Set the background color of the console.
	void Util_ConsoleResetForegroundColor();						// Restore the foreground color of the console to its default value
	void Util_ConsoleResetBackgroundColor();						// Restore the background color of the console to its default value

	void Util_ConsoleClearScreen();									// Clear the console buffer.

	uint64_t Util_GetMicroseconds();									// Get uSec since init
	
}