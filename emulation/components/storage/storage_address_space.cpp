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
    
}
