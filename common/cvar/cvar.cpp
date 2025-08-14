#include <common/common.hpp>

namespace Volt
{
    /*
        ConVar system
        This is basically a linked list but you can't remove items (like in quake, unlike some other games) because we don't really need that
    */

    Cvar* cvarlist_head = nullptr;			// Head of the linked list of cvars.
	Cvar* cvarlist_tail = nullptr;			// Tail of the linked list of cvars.

    /* Functions only used in this translation unit */
    Cvar* Cvar_Exists(const char* name);
	Cvar* Cvar_Create(const char* name, const char* string, CvarFlags flags = Cvar_Default, bool internal = false);

    // Creates a convar. Internal use only.
    Cvar* Cvar_Create(const char* name, const char* string, CvarFlags flags, bool internal)
    {
        Cvar* cvar = Cvar_Exists(name); 

        // check that the cvar exists so we can't have cvars with duplicate names
        if (cvar)
        {
            Logging_LogChannel("Cvar_Create called on already extant cvar %s (this should be a minor engine bug, use Cvar_Set)", LogChannel::Debug, name);
            Cvar_Set(name, string, (flags & CvarFlags::Cvar_NoUser));
            return cvar;
        }

        cvar = Memory_Alloc<Cvar>(TAG_CVAR);

        // if there is no head there are no items
        if (!cvarlist_head)
        {
            cvarlist_head = cvar;
            cvarlist_tail = cvar;
        }
        else
        {
            // optimisation: don't iterate through the entire list
            cvar->prev = cvarlist_tail;

            cvarlist_tail->next = cvar;
            cvarlist_tail = cvar; 

        }

        cvar->name = name;
        cvar->string = string;
        cvar->value = atof(string);

        if (cvar->on_set)
            cvar->on_set(); // call the onset function when the cvar is created

        return cvar; 
    }

    // returns a pointer to the cvar if the cvar with the specified name exists, otherwise a nullptr
    Cvar* Cvar_Exists(const char* name)
    {
        Cvar* cvar = cvarlist_head;

        while (cvar)
        {
            if (!strcasecmp(cvar->name, name))
                return cvar; 

            cvar = cvar->next;
        }

        return nullptr; 
    }

    // Gets the convar with the name. If it doesn't exist, sets its value.
    Cvar* Cvar_Get(const char* name, const char* string, CvarFlags flags, bool internal)
    {
        Cvar* cvar = cvarlist_head;

        while (cvar)
        {
            if (!strcasecmp(cvar->name, name))
            {
                if (cvar->on_get)
                    cvar->on_get();
                    
                return cvar; 
            }
                

            cvar = cvar->next;
        }

        return Cvar_Create(name, string, flags, internal);
    }
   
    // Gets the convar with the name. If it doesn't exist, creates it and sets its value.
    Cvar* Cvar_Get(const char* name, const char* string, bool internal)
    { 
        return Cvar_Get(name, string, (internal) ? Cvar_NoUser : Cvar_Default);
    }

    // Sets the value of a convar.  If it doesn't exist, creates it and sets its value.
    void Cvar_Set(const char* name, const char* string, CvarFlags flags, bool internal)
    {
        Cvar* cvar = cvarlist_head;

        while (cvar)
        {
            if (!strcasecmp(cvar->name, name))
            {
                if (!internal 
                && (cvar->flags & (Cvar_NoUser)))
                    return; /* meh */ 
                
                cvar->string = string;
                cvar->value = atof(cvar->string);

                if (cvar->on_set)
                    cvar->on_set();
                    
                return; 
            }
                
            /* todo: CVAR_ARCHIVE */

            cvar = cvar->next;
        }

        /* Create it if it doesn't exist */
        if (!cvar)
        {
            Logging_LogChannel("Tried to set value of invalid CVar %s. Creating it...\n", LogChannel::Debug, name);
            cvar = Cvar_Create(name, string, flags);
        }

        return; 
    }

    // Set the value of a convar.  If it doesn't exist, creates it and sets its value.
    void Cvar_Set(const char* name, const char* string, bool internal)
    {
        Cvar_Set(name, string, (internal) ? Cvar_NoUser : Cvar_Default);
    }

    // Set the  
    void Cvar_SetOnGetCallback(Cvar* cvar, void (*on_get)())
    {
        if (cvar)
            cvar->on_get = on_get;
    }

	void Cvar_SetOnSetCallback(Cvar* cvar, void (*on_set)())
    {
        if (cvar)
            cvar->on_set = on_set; 
    }

    // Shuts down the convar system.
    void Cvar_Shutdown()
    {
        // no work to do
        if (!cvarlist_head || !cvarlist_tail)
            return; 

        Cvar* cvar = cvarlist_head;

        while (cvar)
        {
           // free in reverse order so we don't made what we're doing invalid
           Cvar* next_free = cvar->next;
           Memory_Free(cvar);
           cvar = next_free; 
        }

        cvarlist_head = nullptr;
        cvarlist_tail = nullptr;
    }
}