// Copyright © 2024 starfrost
// Project: Step7
// Filename: MemoryAlloc.h

// This isn't really a memory manager.
// It just abstracts away malloc()/free() right now, so that if in the future we need an actual memory manager
// we can add one - in order for us to be able to track memory allocations.
#pragma once
#include <Volt.hpp>
#include <common/common.hpp>
#include <util/logging/logging.hpp>

// Memory allocation header so we can see if the memory got corrupted.
#define MEMALLOC_HEADER 				0x7C7C

// Memory tag for filesystem allocations (little endian)
#define TAG_COMMAND     				0x434D4430  // Command system allocations 		'CMD0'
#define TAG_CVAR 						0x43564152	// Convar system allocations		'CVAR'
#define TAG_FILESYSTEM                  0x46494C45 	// Filesystem allocations 			'FILE'  
#define TAG_EMU_ADDR					0x41444452	// Emulation address spaces			'ADDR'
#define TAG_EMU_GUEST_MEM				0x4D454D30	// Emulation address allocations	'MEM0'

#define TAG_EMU_COMPONENT_CPU			0x30555043	// CPU component					'CPU0'
#define TAG_EMU_COMPONENT_GPU			0x30555047	// GPU component					'GPU0'
#define TAG_EMU_COMPONENT_FPU			0x30555046	// FPU component					'FPU0'
#define TAG_EMU_COMPONENT_SND			0x30444E53	// Sound component					'SND0'
#define TAG_EMU_COMPONENT_DMA			0x43414D44	// DMA component					'DMAC'
#define TAG_EMU_COMPONENT_TIMER			0x43524D54	// Timer component					'TMRC'
#define TAG_EMU_COMPONENT_INTERRUPT		0x52544E49	// Interrupt component				'INTR'
#define TAG_EMU_COMPONENT_PERIPHERAL	0x35353238	// Peripheral component				'8255'
#define TAG_EMU_COMPONENT_BUS			0x30535542	// Bus component					'BUS0'
#define TAG_EMU_COMPONENT_FLOPPY		0x504F4C46	// Floppy component					'FLOP'
#define TAG_EMU_COMPONENT_HDD			0x30444448	// HDD component					'HDD0'
#define TAG_EMU_COMPONENT_NET			0x3054454E	// Network component				'NET0'
// More to come...

namespace Volt
{
	template <typename T, int32_t N = 1>
	struct MemAllocHeader; 

	// used for tagged alloc
	template <typename T, int32_t N = 1>
	struct MemAlloc
	{
		MemAllocHeader<T, N> header; 
		T obj[N];
	};

	template <typename T, int32_t N>
	struct MemAllocHeader
	{
		uint16_t magic;
		uint32_t tag;
		uint32_t size; // would we need to allocate above 4tb??
	  	MemAlloc<T, N>* prev;
		MemAlloc<T, N>* next;
	};

	extern uint32_t memalloc_count;

	extern void* memalloc_head;
	extern void* memalloc_tail;

	extern uint32_t memalloc_total_size;

	template <typename T, int32_t N = 1>
	T* Memory_Alloc(uint32_t tag)
	{
		// allocate +sizeof(MemAlloc) for tag
		MemAlloc<T, N>* alloc = new MemAlloc<T, N>();

		if (!alloc)
		{
			Logging_LogChannel("Memory_Alloc new call failed size=%d tag=%d", LogChannel::SuperFatal, sizeof(T), tag);
			return nullptr;
		}

		alloc->header.magic = MEMALLOC_HEADER;
		alloc->header.tag = tag;
		alloc->header.size = sizeof(T);

		MemAlloc<T, N>* head = (MemAlloc<T, N>*)memalloc_head;
		MemAlloc<T, N>* tail = (MemAlloc<T, N>*)memalloc_tail;

		// if we don't have any allocations, set the tail to this
		if (memalloc_count > 0)
		{
			alloc->header.prev = tail;
			tail->header.next = alloc;
		}
		else
		{
			alloc->header.prev = nullptr;
			alloc->header.next = nullptr;
			memalloc_tail = (void*)alloc;
		}
			
		memalloc_count++;
		memalloc_total_size += sizeof(T);

		// set the head of the list if it is not present
		if (!memalloc_head)
			memalloc_head = (void*)alloc;

		// move around the tail of the list
		memalloc_tail = (void*)alloc;

		// todo: cvar
		
		Logging_LogChannel("Dynamically allocated %d bytes tag=0x%04X", LogChannel::Debug, sizeof(T), tag);

		// this is a pointer to a memalloc_t*, so we need to simply increment it to skip past the magic
		return alloc->obj; // handle word size
	}
	
	template <typename T>
	void Memory_Free(T* ptr)
	{
		// first see if there is anything to free
		if (memalloc_count == 0
			|| memalloc_head == nullptr)
		{
			Logging_LogChannel("Tried to free memory with no memory allocated", LogChannel::SuperFatal);
		}

		if (!ptr)
		{
			Logging_LogChannel("Tried to call Memory_Free with nullptr", LogChannel::SuperFatal); // does not return
		}

		// get the pointer to the memalloc structure
		// this is also what we actually free
		MemAlloc<T>* memalloc = (MemAlloc<T>*)memalloc_head;

		// see if this is a valid allocation by seeing if the object is in it
		while (memalloc
			&& memalloc->obj != ptr)
		{
			// go to the next one
			memalloc = memalloc->header.next;
		}

		// there is no memalloc, double free attempted
		if (!memalloc)
		{
			Logging_LogChannel("Tried to double-free memory. Heap corruption", LogChannel::SuperFatal);
			return; //shutup compiler
		}

		// see if we actually allocated the memory
		if (memalloc->header.magic != MEMALLOC_HEADER)
			Logging_LogChannel("Memory_Free - Tried to free invalid memory (MEMALLOC_HEADER not found)\nBasically, we're going down.", LogChannel::SuperFatal); // does not reutrn

		// fix the head if we were the head
		if (!memalloc->header.next
			&& memalloc != memalloc_tail) //hosed, head isn't the head?
		{
			Logging_LogChannel("Memory_Free - Broken allocation chain. Going down to prevent catastrophic failure", LogChannel::SuperFatal); // does not return
		}
		else if (memalloc == memalloc_tail)
			memalloc_tail = (void*)memalloc->header.prev;
		else if (memalloc == memalloc_head)
			memalloc_head = (void*)memalloc->header.next;

		// remove us from the chain if we are not the head
		else if (memalloc != memalloc_tail
			&& memalloc->header.prev)
		{
			if (memalloc->header.prev)
				memalloc->header.prev->header.next = memalloc->header.next;

			if (memalloc->header.next)
				memalloc->header.next->header.prev = memalloc->header.prev;
		}

		Logging_LogChannel("About to free %d bytes at 0x%p tag=0x%08X", LogChannel::Debug, memalloc->header.size, (void*)memalloc, memalloc->header.tag);

		memalloc_total_size -= memalloc->header.size;
		memalloc_count--;

		// if there are now no memallocs, there is no head
		if (memalloc_count <= 0)
			memalloc_head = nullptr; 

		// free
		delete memalloc;
	}
}

