// 
// VOLT
// Copyright © 2025 starfrost
//
// render_display.hpp: Display system definition for Volt emulator
//

#pragma once
#include <render/render.hpp>

namespace Volt
{
    // System Monitor list
    class Display
    {
    public:
        void SetSize(Vector2i size);
        
    private: 
        Vector2i size; 

    };

}