//
// Emulation Core
//

#pragma once
#include <common/common.hpp>
#include <emulation/component.hpp>
#include <emulation/machine.hpp>
#include <emulation/components/io/io_port.hpp>
#include <emulation/components/storage/storage_address_space.hpp>

namespace Volt
{
    void Emulation_Init();
    void Emulation_Start();
    void Emulation_Tick();
    void Emulation_Shutdown();

    // Test machine
    extern Machine test_machine;
}