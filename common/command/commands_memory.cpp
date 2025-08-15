#include <common/command/command.hpp>

//
// commands_memory.cpp
// Implements the memory commands. 
//

namespace Volt
{
    void Command_MemStats(CommandType origin)
    {
        Logging_LogChannel("%d allocations (total memory use: %d bytes)", LogChannel::Message, memalloc_count, memalloc_total_size);
    }

    // List all memory allocations
    void Command_ListAllocs(CommandType origin)
    {
        // it does not matter we just need to report the size so we can just convert it to any type
        MemAlloc<int32_t, 1>* alloc_current = (MemAlloc<int32_t, 1>*)memalloc_head;

        uint32_t alloc_num = 0;

        while (alloc_current)
        {
            Logging_LogChannel("Alloc %d: %.4c (%d bytes)", LogChannel::Message, alloc_num, (char*)alloc_current->header.tag, alloc_current->header.size);
            alloc_current = alloc_current->header.next; 
            alloc_num++; 
        }
        
        Logging_LogChannel("%d allocations (total memory use: %d bytes)", LogChannel::Message, memalloc_count, memalloc_total_size);

    }
}