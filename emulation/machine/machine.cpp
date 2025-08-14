#include <emulation/emulation.hpp>

//
// machine.cpp: Implements Machine class
//

namespace Volt
{
    template <typename T>
    void Machine::AddComponent(T* component)
    {   
        // Ensure we are actually adding a component
        static_assert(component == dynamic_cast<Component*>(component), "Machine::AddComponent requires allocated component");

        components.push_back(component);       
        
        return; 
    }

    void Machine::Tick()
    {
        for (auto component : components)
        {
            if (component->update)
            {
                component->Tick();
            }
        }
    }

    Machine test_machine;

    #ifdef DEBUG

    void Machine::DebugInit()
    {
        
    }

    #endif
}