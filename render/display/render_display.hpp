// 
// VOLT
// Copyright © 2025 starfrost
//
// render_display.hpp: Display system definition for Volt emulator
//

namespace Volt
{
    // System Monitor list
    class Monitor
    {
    public:
        void SetSize(uint32_t x, uint32_t y);
        
    private: 
        uint32_t x;
        uint32_t y; 

    };

}