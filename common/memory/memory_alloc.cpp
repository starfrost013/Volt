#include <common/common.hpp>
#include <common/memory/memory_alloc.hpp>
namespace Volt
{
	void* memalloc_head = nullptr;	// The head of the allocation list

	void* memalloc_tail = nullptr;	// The tail of the allocation list

	uint32_t memalloc_count = 0;
	size_t
	 memalloc_total_size = 0;
}