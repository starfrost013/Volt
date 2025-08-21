// Volt Research Emulator
//
// Copyright © 2024-2025 starfrost / Connor Hyde
// Portions  © 2023-2025 starfrost / Connor Hyde
//
// Volt.cpp: The main engine entry point

#include <Volt.hpp>

namespace Volt
{
	int32_t engine_entry(int32_t argc, char* argv[])
	{
		 // Seed the random number generator.
		srand(time(NULL));

		/* Common Initialisation */
		Common_Init(argc, argv);

		Common_Shutdown();		
		return 0;
	}
}

// Main function - stub that calls engine_entry 
int32_t main(int32_t argc, char* argv[])
{
	// jump into the entry point
	Volt::engine_entry(argc, argv);
}
