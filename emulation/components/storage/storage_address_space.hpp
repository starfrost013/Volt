//
// storage_address_space.hpp: Defines a CPU's Address Space that RAM or memory-mapped I/O can be mapped into (21 July 2025)
// Implements a singly-linked list of address spaces which is returned to the requesting device.
// There is also a 'primary' address space (which is also the head that is assigned to the primary CPU. This is for devices with their own spaces such as the IBM Professional Graphics Controller
//

#pragma once
#include <common/common.hpp>
#include <emulation/emulation.hpp>
#include <common/memory/memory_alloc.hpp>   // Memory allocator needs to be here

namespace Volt
{
    // Map nonpr
    struct AddressSpaceMapping
    {
        uint32_t start; 
        uint32_t end;
        Component* component;  

        AddressSpaceMapping* next;
    };

    // Various mechanisms for access of the address space.
    class AddressSpace
    {
        public: 
            uint32_t size; 
            uint8_t* access_raw;
            bool is_primary; 

            AddressSpaceMapping* mapping_head;      // Only valid if is_primary is true
            AddressSpaceMapping* mapping_tail;      // Only valid if is_primary is true

            AddressSpace* next; 

            uint8_t read_byte(uint32_t index);
            uint16_t read_word(uint32_t index);
            uint32_t read_dword(uint32_t index);

            void write_byte(uint32_t index, uint8_t value);
            void write_word(uint32_t index, uint16_t value);
            void write_dword(uint32_t index, uint32_t value);

            void AddMapping(uint32_t start, uint32_t end, Component* comp);
            bool AddressIsMapped(uint32_t addr);
            Component* AddressGetComponent(uint32_t addr);
            void Shutdown();
    };

    extern AddressSpace* address_space_primary; 

    //
    // Global function stuff
    //
    
    template <uint32_t Size>
    AddressSpace* AddressSpace_Add()
    {
        AddressSpace* addr = Memory_Alloc<AddressSpace>(TAG_EMU_ADDR);

        if (!addr)
        {
            Logging_LogChannel("Failed to allocate address space\n", LogChannel::Fatal);
            return nullptr; 
        }

        addr->access_raw = Memory_Alloc<uint8_t, Size>(TAG_EMU_GUEST_MEM);
        addr->size = Size;

        if (!address_space_primary)
        {
            address_space_primary = addr; 
        }
        else 
        {
            AddressSpace* last_addr = address_space_primary;

            while (last_addr->next)
                last_addr = last_addr->next; 

            last_addr->next = addr; 

        }

        addr->is_primary = (address_space_primary == addr);

        return addr; 
    };

    void AddressSpace_Shutdown(AddressSpace* ptr);
}
