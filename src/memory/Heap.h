#pragma once

//#define HEAPDEBUG

#ifndef HEAP_H_
#define HEAP_H_

#include "GarbageCollector.h"
#include "../misc/defs.h"
#include "../vmobjects/ObjectFormats.h"
//#include "../misc/HashMap.h"
class VMObject;

struct free_list_entry 
{
    free_list_entry* next;
    size_t size;
} ;


class Heap
{
	friend class GarbageCollector;

public:
    static Heap* GetHeap();
    static void InitializeHeap(int objectSpaceSize = 1048576);
    static void DestroyHeap();
	Heap(int object_space_size = 1048576);
	~Heap();
    pVMObject AllocateObject(size_t size);
	void* Allocate(size_t size);
    void Free(void* ptr);
	void Free(void* ptr, int size);
	
    void StartUninterruptableAllocation() { ++uninterruptable_counter; } ;
    void EndUninterruptableAllocation() { --uninterruptable_counter; } ;

	//void SetGlobal(StdString name, void* val);
    
private:
    static Heap* theHeap;

    void internalFree(void* ptr);
	void* internalAllocate(size_t size);

	void* object_space;

	free_list_entry* free_list_start;
    
    
	
    int object_space_size;
	int buffersize_for_uninterruptable;
	int uninterruptable_counter;
	int size_of_free_heap;

	GarbageCollector *gc;

    uint32_t num_alloc;
    uint32_t spc_alloc;
    uint32_t num_alloc_total;
	//HashMap<StdString, void*, OOObject> globals;
};

#endif
