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

    // Get around stupid include crap
    void Machine::LogComponent(const char* name)
    {
        Logging_LogChannel("Successfully added machine component %s", Volt::LogChannel::Debug, name);
    }
}