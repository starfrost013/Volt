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
        uint32_t size; 
        uint8_t* access_byte;

        inline uint16_t access_word(uint32_t index) { return (access_byte[index + 1] << 8 | access_byte[index]); };

        inline uint32_t access_dword(uint32_t index) 
        { 
            return access_byte[index + 3] << 24 | access_byte[index + 2] << 16
            | access_byte[index + 1] << 8 | access_byte[index];
        };

        AddressSpace* next; 
    };

    extern AddressSpace* address_space_primary; 

    template <uint32_t Size>
    AddressSpace* AddressSpace_Add()
    {
        AddressSpace* addr = Memory_Alloc<AddressSpace>(TAG_EMU_ADDR);

        if (!addr)
        {
            Logging_LogChannel("Failed to allocate address space\n", LogChannel::Fatal);
            return nullptr; 
        }

        addr->access_byte = Memory_Alloc<uint8_t, Size>(TAG_EMU_GUEST_MEM);;
        addr->size = Size;

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
