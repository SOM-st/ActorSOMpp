#pragma once

#ifndef GARBAGECOLLECTOR_H_
#define GARBAGECOLLECTOR_H_

class Heap;

class VMObject;
#include "../misc/defs.h"

class GarbageCollector
{
public:
	GarbageCollector(Heap* h);
	~GarbageCollector();
	void Collect();
    void PrintGCStat();
    void PrintCollectStat();
	

private:
	void markReachableObjects();
	void markObject(VMObject* obj);
	void mergeFreeSpaces();
	Heap* heap;

    //
    // values for GC statistics
    //
    uint32_t num_collections;
	uint32_t num_live;
	uint32_t spc_live;
	uint32_t num_freed;
	uint32_t spc_freed;

	

};

#endif
