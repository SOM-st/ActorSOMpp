#pragma once

#ifndef GARBAGECOLLECTOR_H_
#define GARBAGECOLLECTOR_H_

class Heap;
#include "../vmobjects/ObjectFormats.h"
class VMObject;
#include "../misc/defs.h"

class GarbageCollector {
public:
	GarbageCollector(Heap* h);
	~GarbageCollector();
	void Collect();
    void PrintGCStat() const;
    void PrintCollectStat() const;
	

private:
	void markReachableObjects();
	void markObject(pVMObject obj);
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
