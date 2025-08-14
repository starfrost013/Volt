#pragma once
#include <common/common.hpp>

//
// cmdline.hpp
// Contains basic command line parsing functions.
//

namespace Volt
{
	void Cmdline_Init(int32_t argc, char** argv);
	bool Cmdline_Check(const char* option);
	uint32_t Cmdline_Argc();
	const char* Cmdline_Argv(uint32_t c);
}