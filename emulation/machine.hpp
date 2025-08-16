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
            void Tick();

            // Should these components be dynamically allocated?
            template <typename T>
            void AddComponent(T* component)
            {   
                // Ensure we are actually adding a component
                //assert(component == dynamic_cast<Component*>(component), "Machine::AddComponent requires allocated component");

                components.push_back(component);     
                
                component->Init();
                
                return; 
            }

            void Start()
            {
                for (auto component : components)
                {
                    component->Start();
                }
            }

            std::vector<Component*> components;
            
            Machine()
            {
                components.reserve(RESERVED_COMPONENTS);
            }
        protected: 
        
        
    };
}

