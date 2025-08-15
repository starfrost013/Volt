#include <emulation/emulation.hpp>

//
// machine.cpp: Implements Machine class
//

namespace Volt
{


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
}