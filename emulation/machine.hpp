// 
// VOLT
// Copyright © 2025 starfrost
//
// machine.hpp: Defines a machine
//
#pragma once
#include <emulation/emulation.hpp>

#define RESERVED_COMPONENTS     16

namespace Volt
{
    class Machine
    {
        public: 
            // Should these components be dynamically allocated?
            template <typename T>
            void AddComponent(T* component)
            {   
                // Ensure we are actually adding a component
                //assert(component == dynamic_cast<Component*>(component), "Machine::AddComponent requires allocated component");

                components.push_back(component);     
                
                component->Init(this);

                // gets around include nonsense
                Machine::LogComponent(component->name);

                return; 
            }

            template <typename T>
            T* FindComponentByClass()
            {
                for (auto component : components)
                {
                    T* derived = dynamic_cast<T*>(component);

                    if (derived)
                        return derived; 
                }
                
                return nullptr; 
            }

            void Start()
            {
                // start stuff like roms so they can be mapped into cpu address space
                for (auto component : components)
                {
                    if (component->early_start)
                        component->Start();
                }

                // start non-early start components
                for (auto component : components)
                {
                    if (!component->early_start)
                        component->Start();
                }
            }

            void Tick()
            {
                for (auto component : components)
                {
                    if (component->update)
                    {
                        // make it optional to initialise microseconds_target
                        if (!component->microseconds_target)
                            component->microseconds_target = (1 / double(component->clock_hz) * 1000000.0f);

                        // various measurements
                        double real_milliseconds = 0, target_milliseconds = 0, render_milliseconds = 0, update_milliseconds = 0;
                        double start = 0, end = 0;

                        start = Util_GetMicroseconds(); 

                        bool run_tick = (component->microseconds_current > component->microseconds_target);

                        if (component->clock_skip > 0)
                            run_tick = (component->microseconds_current > (component->microseconds_target * double(component->clock_skip)));

                        if (run_tick)
                            component->Tick();

                        end = Util_GetMicroseconds(); 
                        
                        if (run_tick)
                            component->microseconds_current = double(end - start);
                        else
                            component->microseconds_current += double(end - start);
                    }
                }
            }

            void Shutdown()
            {
                for (auto component : components)
                {
                    component->Shutdown();
                    //Memory_Free<Component>(component);
                }
            }

            // Get around stupid include crap
            static void LogComponent(const char* name)
            {
                Logging_LogChannel("Successfully added machine component %s", Volt::LogChannel::Debug, name);
            }

            std::vector<Component*> components;
            
            Machine()
            {
                components.reserve(RESERVED_COMPONENTS);
            }
        protected: 
        
        
    };
}

