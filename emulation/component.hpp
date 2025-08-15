// 
// VOLT
// Copyright © 2025 starfrost
//
// component.hpp: The class for a machine component
//
#pragma once
#include <emulation/emulation.hpp>

namespace Volt
{
    class Component
    {
        public:
            const char* name;

            // Does this component need to be updated? 
            // A CPU needs to be ticked.
            // A printer card does not need to be ticked.
            bool update; 
            
            virtual void Init();
            virtual void Tick();
            virtual void Frame();       // Video only
            virtual void Shutdown();
        protected:
        private: 
    };
}
