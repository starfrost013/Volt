#include "util.hpp"

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// DON'T INCLUDE THIS ANYWHERE ELSE IT BREAKS THE ENGINE DESIGN
#include <GLFW/glfw3.h>

namespace Volt
{
	const char* STRING_EMPTY = "";
	const char* STRING_SINGLE_SPACE = " ";

	void Util_DateGetCurrentString(char *str)
	{
		time_t current_time = 0;
		// get the local time
		time(&current_time);
		const struct tm* current_time_info_ptr = localtime(&current_time); // todo: use localtime

		assert(str);

		char* final_text = asctime(current_time_info_ptr);

		assert(final_text);

		if (!final_text) 
			return;

		strcpy(str, final_text);
	}

	bool Util_BooleanFromString(char* str)
	{
		assert(str != NULL);

		str = strlwr(str);

		if (strncmp(str, "true", 4))
			return true;
		else if (strncmp(str, "false", 5))
			return false;

		// everything else is false
		return false;
	}

	char* Util_GetTokenSeparatedPart(char* fmt, const char* delim, uint32_t n)
	{
		/* sanity checks */

		if (!fmt ||  !delim)
			return nullptr; 

		size_t fmt_len = strlen(fmt);

		if (fmt_len < 0)
			return nullptr;

		char* tok = strtok(fmt, delim);

		for (uint32_t tok_id = 0; tok_id < n; tok_id++)
		{
			tok = strtok(NULL, delim);

			if (!tok)
				break; 
		}

		return tok;
	}

	uint64_t Util_GetMicroseconds()
	{
		return glfwGetTime() * 1000000.0;
	}
}
