//
// storage_address_space.cpp: Implements a CPU's Address Space that RAM or memory-mapped I/O can be mapped into (21 July 2025)
// Implements a singly-linked list of address spaces which is returned to the requesting device.
// There is also a 'primary' address space (which is also the head that is assigned to the primary CPU. This is for devices with their own spaces such as the IBM Professional Graphics Controller
//
#include <emulation/components/storage/storage_address_space.hpp>

namespace Volt
{
    // The "Primary" address space of the device
    AddressSpace* address_space_primary; 
    // Let's hope everything checks the size so this isn't too slow.

    // This needs to be here until includes get sorted out
    extern Cvar* emu_system_ram; 

    uint8_t AddressSpace::read_byte(uint32_t index)
    {
        // large positive indexes are handled below
        if (index < 0)
        {
            Logging_LogChannel("Invalid read_byte index, nothing will happen", LogChannel::Error);
            return 0xFF;
        }
        if (index > size) 
            Logging_LogChannel("Invalid read_byte index, the read will go to the wrong place", LogChannel::Error);

        Component* comp = AddressGetComponent(index);

        // if the address is mapped, immediately return
        // TODO: read_function
        if (comp && comp->allow_device_reads)
            return access_byte[(index & (size - 1))];
    
        if (is_primary)
            return (index >= emu_system_ram->value) ? 0xFF : access_byte[index];
        else
            return access_byte[index];
    }

    void AddressSpace::write_byte(uint32_t index, uint8_t value)
    {
        // large positive indexes are handled below
        if (index < 0)
        {
            Logging_LogChannel("Invalid write_byte index, nothing will happen", LogChannel::Error);
            return;
        }
        if (index > size) 
            Logging_LogChannel("Invalid write_byte index, the read will go to the wrong place", LogChannel::Error);

        Component* comp = AddressGetComponent(index);

        //TODO: read_function
        if (comp && comp->allow_device_writes)
            access_byte[index] = value;
        else
        {
            if (is_primary)
                if (index >= emu_system_ram->value)
                    return;
                else
                    access_byte[index] = value;
            else
                access_byte[index] = value;
        }

    }

    uint16_t AddressSpace::read_word(uint32_t index) 
    { 
        // large positive indexes are handled below
        if (index < 0)
        {
            Logging_LogChannel("Invalid read_word index, nothing will happen", LogChannel::Error);
            return 0xFF;
        }
        if (index > size) 
            Logging_LogChannel("Invalid read_word index, the read will go to the wrong place", LogChannel::Error);

        uint16_t ret = 0x00; 
        Component* comp = AddressGetComponent(index);

        // if the address is mapped, immediately return
        // TODO: read_function
        if (comp && comp->allow_device_reads)
            ret = (access_byte[index + 1] << 8 | access_byte[index]);  
        else
        {
            if (is_primary)
            {
                ((index % size) >= (emu_system_ram->value)) ? ret |= 0xFF00 : ret |= (access_byte[index + 1] << 8);
                ((index % size) >= emu_system_ram->value - 1) ? ret |= 0xFF : ret |= (access_byte[index]);
            }
            else
                ret = (access_byte[index + 1] << 8 | access_byte[index]);
        }

    
        return ret; 
    };

    // write a u16 to memory
    void AddressSpace::write_word(uint32_t index, uint16_t value) 
    { 
        // large positive indexes are handled below
        if (index < 0)
        {
            Logging_LogChannel("Invalid write_dword index, nothing will happen", LogChannel::Error);
            return;
        }
        if (index > size) 
            Logging_LogChannel("Invalid write_dword index, the write will go to the wrong place", LogChannel::Error);

        Component* comp = AddressGetComponent(index);

        if (comp && comp->allow_device_writes)
        {            
            access_byte[index + 1] = (value >> 8);
            access_byte[index] = value & 0xFF;
        }
        else 
        {
            if ((index % size) >= (emu_system_ram->value)) access_byte[((index + 1) % size)] = (value >> 8);
            if ((index % size) >= (emu_system_ram->value) - 1) access_byte[(index % size)] = value & 0xFF;
        }
    };

    uint32_t AddressSpace::read_dword(uint32_t index) 
    { 
        // large positive indexes are handled below
        if (index < 0)
        {
            Logging_LogChannel("Invalid read_dword index, nothing will happen", LogChannel::Error);
            return 0xFF;
        }
        if (index > size) 
            Logging_LogChannel("Invalid read_dword index, the write will go to the wrong place", LogChannel::Error);

        uint32_t ret = 0x00; 
        Component* comp = AddressGetComponent(index);
        
        if (comp && comp->allow_device_reads)
        {
            ret = access_byte[index + 3] << 24 | access_byte[index + 2] << 16
            | access_byte[index + 1] << 8 | access_byte[index];
        }
        else
        {
            if (is_primary)
            {
                ((index & (size - 1)) >= (emu_system_ram->value)) ? ret |= 0xFF000000 : ret |= (access_byte[index + 3] << 24);
                ((index & (size - 1)) >= (emu_system_ram->value) - 1) ? ret |= 0xFF0000 : ret |= (access_byte[index + 2] << 16);
                ((index & (size - 1)) >= (emu_system_ram->value) - 2) ? ret |= 0xFF00 : ret |= (access_byte[index + 1] << 8);
                ((index & (size - 1)) >= emu_system_ram->value - 3) ? ret |= 0xFF : ret |= (access_byte[index]);
            }
            else
                ret = access_byte[index + 3] << 24 | access_byte[index + 2] << 16
                | access_byte[index + 1] << 8 | access_byte[index];
        }
            
        return ret; 
    };

    void AddressSpace::write_dword(uint32_t index, uint32_t value)
    {
        // large positive indexes are handled below
        if (index < 0)
        {
            Logging_LogChannel("Invalid write_dword index, nothing will happen", LogChannel::Error);
            return;
        }
        if (index > size) 
            Logging_LogChannel("Invalid write_dword index, the write will go to the wrong place", LogChannel::Error);

        Component* comp = AddressGetComponent(index);

        if (comp && comp->allow_device_writes)
        {
            if ((index % size) >= (emu_system_ram->value)) access_byte[((index + 3) % size)] = (value >> 24);
            if ((index % size) >= (emu_system_ram->value) - 1) access_byte[((index + 2) % size)] = (value >> 16) & 0xFF;
            if ((index % size) >= (emu_system_ram->value) - 2) access_byte[((index + 1) % size)] = (value >> 8) & 0xFF;
            if ((index % size) >= (emu_system_ram->value) - 3) access_byte[(index % size)] = value & 0xFF;
        }
        else
        {
            access_byte[((index + 3) % size)] = (value >> 24);
            access_byte[((index + 2) % size)] = (value >> 16) & 0xFF;
            access_byte[((index + 1) % size)] = (value >> 8) & 0xFF;
            access_byte[(index % size)] = value & 0xFF;
        }


    };

    void AddressSpace::AddMapping(uint32_t start, uint32_t end, Component* comp)
    {
        if (!is_primary)
            return; 
        
        if (end < start)
        {
            Logging_LogChannel("Failed to add primary address space mapping because end < start", LogChannel::Warning);
            return;    
        }

        AddressSpaceMapping* mapping = Memory_Alloc<AddressSpaceMapping>(TAG_EMU_ADDR_MAP);

        if (!mapping_head && !mapping_tail)
            mapping_head = mapping_tail = mapping;
        else
        {
            mapping_tail->next = mapping;
            mapping_tail = mapping;
        }

        mapping->start = start;
        mapping->end = end;
        mapping->component = comp;
    }

    bool AddressSpace::AddressIsMapped(uint32_t addr)
    {
        if (!is_primary)
            return false;

        AddressSpaceMapping* mapping = mapping_head;

        while (mapping)
        {
            if (addr >= mapping->start
            && addr <= mapping->end)
            {
                return true;
            }

            mapping = mapping->next; 
        }

        return false; 
    }

    Component* AddressSpace::AddressGetComponent(uint32_t addr)
    {
        if (!is_primary)
            return nullptr;

        AddressSpaceMapping* mapping = mapping_head;  

        while (mapping)
        {
            if (addr >= mapping->start
            && addr <= mapping->end)
            {
                return mapping->component;
            }

            mapping = mapping->next; 
        }

        return nullptr;     
    }
    
    void AddressSpace::Shutdown()
    {
        if (is_primary)
        {
            // clear out all mappings
            AddressSpaceMapping* current = mapping_head;

            while (current)
            {
                AddressSpaceMapping* next = current->next; 

                Memory_Free<AddressSpaceMapping>(current);

                current = next;
            }
        }
    }



    void AddressSpace_Shutdown(AddressSpace* ptr)
    {
        ptr->Shutdown();

        Memory_Free<AddressSpace>(ptr);
    }
}
