#pragma once
#include <common/common.hpp>

// 
// Cvar.hpp: Defines functions for convars.
//

namespace Volt
{
	enum CvarFlags
	{
		Cvar_Default = 0,

		// User can't write to this cvar, only the engine
		Cvar_NoUser = 1,

		// This cvar is saved to the configuration.
		Cvar_Archive = 1 << 1,

		// Edits to this cvar only have any effect on the server.
		Cvar_Replicate = 1 << 2,
	};

	struct Cvar
	{
		const char* name;
		const char* string;
		CvarFlags flags; // could be a float, string, blah blah...
		float value;
		Cvar* prev;
		Cvar* next;
		void (*on_get)();
		void (*on_set)();
	};

	Cvar* Cvar_Get(const char* name, const char* string = "", bool internal = false);
	Cvar* Cvar_Get(const char* name, const char* string = "", CvarFlags flags = Cvar_Default, bool internal = false);
    void Cvar_Set(const char* name, const char* string = "", bool internal = false);
	void Cvar_Set(const char* name, const char* string = "", CvarFlags flags = Cvar_Default, bool internal = false);
    void Cvar_List();
	
	// Functions for ancillary functionality it doesn't make sense to stuff into parameters.
	void Cvar_SetOnGetCallback(Cvar* cvar, void (*on_get)());
	void Cvar_SetOnSetCallback(Cvar* cvar, void (*on_set)());
	void Cvar_Shutdown();
}
