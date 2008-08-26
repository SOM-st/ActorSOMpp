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

	

private:
	void markReachableObjects();
	void markObject(VMObject* obj);
	void mergeFreeSpaces();
	Heap* heap;
	uint32_t num_live;
	uint32_t spc_live;
	uint32_t num_freed;
	uint32_t spc_freed;
	

};

#endif