#pragma once

#ifndef GARBAGECOLLECTOR_H_
#define GARBAGECOLLECTOR_H_


#include "../vmobjects/ObjectFormats.h"
#include "../misc/defs.h"

class VMObject;
class Heap;

class GarbageCollector {
public:
	GarbageCollector(Heap* h);
	~GarbageCollector();
	void Collect();
    void PrintGCStat() const;
    void PrintCollectStat() const;
	

private:
	void markReachableObjects();
	void mergeFreeSpaces();
	Heap* heap;

    //
    // values for GC statistics
    //
    uint32_t numCollections;
	uint32_t numLive;
	uint32_t spcLive;
	uint32_t numFreed;
	uint32_t spcFreed;

	

};

#endif
