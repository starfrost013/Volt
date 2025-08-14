#ifdef _WIN32
// 
// sys_win32.cpp : Holds system-specific win32 stuff.
//
#include <sys/windows/sys_win32.hpp>

namespace Volt
{

	char* Sys_GetCommandLine()
	{
		return GetCommandLine();
	}
}

#endif