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
            void AddComponent(T* component);

            std::vector<Component*> components;
            
            Machine()
            {
                components.reserve(RESERVED_COMPONENTS);
            }

            #ifdef DEBUG

            // Internal initialisation entry point for Pre-Alpha builds
            void DebugInit();

            #endif 
        protected: 
        
        
    };
}

