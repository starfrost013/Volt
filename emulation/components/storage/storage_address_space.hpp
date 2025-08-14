//
// storage_address_space.hpp: Defines a CPU's Address Space that RAM or memory-mapped I/O can be mapped into (21 July 2025)
// Implements a singly-linked list of address spaces which is returned to the requesting device.
// There is also a 'primary' address space (which is also the head that is assigned to the primary CPU. This is for devices with their own spaces such as the IBM Professional Graphics Controller
//

#pragma once
#include <common/memory/memory_alloc.hpp>
#include <emulation/emulation.hpp>

namespace Volt
{
    // Various mechanisms for access of the address space.
    struct AddressSpace
    {
        uint8_t* access_byte;
        uint16_t* access_word;
        uint32_t* access_dword;
        AddressSpace* next; 
    };

    extern AddressSpace* address_space_primary; 

    template <uint32_t Size>
    AddressSpace* AddressSpace_Add()
    {
        constexpr int32_t v = 1000000;

        AddressSpace* addr = Memory_Alloc<AddressSpace>(TAG_EMU_ADDR);

        if (!addr)
        {
            Logging_LogChannel("Failed to allocate address space\n", LogChannel::Fatal);
            return nullptr; 
        }

        addr->access_byte = Memory_Alloc<uint8_t, Size>(TAG_EMU_GUEST_MEM);
        addr->access_word = (uint16_t*)addr->access_byte;
        addr->access_dword = (uint32_t*)addr->access_byte;
        
        if (!address_space_primary)
            address_space_primary = addr; 
        else 
        {
            AddressSpace* last_addr = address_space_primary;

            while (last_addr->next)
                last_addr = last_addr->next; 

            last_addr->next = addr; 

        }

        return addr; 
    }
}
