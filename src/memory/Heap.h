#pragma once

//#define HEAPDEBUG

#ifndef HEAP_H_
#define HEAP_H_

#include "GarbageCollector.h"
#include "../misc/defs.h"
//#include "../misc/HashMap.h"
class VMObject;

struct free_list_entry {
    free_list_entry* next;
    size_t size;
} ;


class Heap
{
	friend class GarbageCollector;

public:
	Heap(int object_space_size = 1000000);
	~Heap();
    VMObject* AllocateObject(size_t size);
	void* Allocate(size_t size);
    void Free(void* ptr);
	void Free(void* ptr, int size);
	
	void GCCollect() {gc->Collect();}
	//void SetGlobal(pString name, void* val);
int allocCount;
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
