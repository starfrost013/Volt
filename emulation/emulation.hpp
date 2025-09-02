// 
// VOLT
// Copyright © 2025 starfrost
//
// emulation.hpp: Core emulation definitions
//

#pragma once
#include <common/common.hpp>
#include <util/logging/logging.hpp>
#include <util/util.hpp>
#include <emulation/component.hpp>
#include <emulation/components/support/component_dma_controller.hpp>
#include <emulation/components/support/component_interrupt_controller.hpp>
#include <emulation/components/support/component_keyboard_controller.hpp>
#include <emulation/machine.hpp>
#include <emulation/components/io/io_port.hpp>
#include <emulation/components/storage/storage_address_space.hpp>

namespace Volt
{
    void Emulation_Init();
    void Emulation_InitCommands();
    void Emulation_Start();
    void Emulation_Tick();
    void Emulation_Shutdown();

    // Test machine
    extern Machine emu_machine;

    extern std::thread* emu_thread;
}