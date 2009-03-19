#pragma once

//#define HEAPDEBUG

#ifndef HEAP_H_
#define HEAP_H_

#include "GarbageCollector.h"

#include "../misc/defs.h"

#include "../vmobjects/ObjectFormats.h"

class VMObject;

//macro to access the heap
#define _HEAP Heap::GetHeap()

struct FreeListEntry 
{
    FreeListEntry* next;
    size_t size;
} ;


class Heap
{
	friend class GarbageCollector;

public:
    static Heap* GetHeap();
    static void InitializeHeap(int objectSpaceSize = 1048576);
    static void DestroyHeap();
	Heap(int objectSpaceSize = 1048576);
	~Heap();
    pVMObject AllocateObject(size_t size);
	void* Allocate(size_t size);
    void Free(void* ptr);
	void Free(void* ptr, int size);
	
    void StartUninterruptableAllocation() { ++uninterruptableCounter; } ;
    void EndUninterruptableAllocation() { --uninterruptableCounter; } ;

	//void SetGlobal(StdString name, void* val);
    
private:
    static Heap* theHeap;

    void internalFree(void* ptr);
	void* internalAllocate(size_t size);

	void* objectSpace;

	FreeListEntry* freeListStart;
    
    
	
    int objectSpaceSize;
	int buffersizeForUninterruptable;
	int uninterruptableCounter;
	int sizeOfFreeHeap;

	GarbageCollector* gc;

    uint32_t numAlloc;
    uint32_t spcAlloc;
    uint32_t numAllocTotal;
};

#endif
