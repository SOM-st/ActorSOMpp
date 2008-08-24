#pragma once

#ifndef HEAP_H_
#define HEAP_H_

#include "GarbageCollector.h"
#include "../misc/defs.h"
#include "../misc/HashMap.h"

typedef struct _free_list_entry free_list_entry;
struct _free_list_entry {
    free_list_entry* next;
    size_t size;
} ;


class Heap
{
public:
	Heap(int object_space_size = 1000000);
	~Heap();
	void* Allocate(size_t size);
	void Free(void* ptr);

	//void SetGlobal(pString name, void* val);

private:
	void* internalAllocate(size_t size);

	void* object_space;

	free_list_entry* free_list_start;

	int object_space_size;
	int buffersize_for_uninterruptable;
	int uninterruptable_counter;
	int size_of_free_heap;

	GarbageCollector *gc;

	//HashMap<pString, void*, OOObject> globals;
};

#endif