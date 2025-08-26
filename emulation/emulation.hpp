//
// Emulation Core
//

#pragma once
#include <common/common.hpp>
#include <util/logging/logging.hpp>
#include <util/util.hpp>
#include <emulation/component.hpp>
#include <emulation/components/support/component_dma_controller.hpp>
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