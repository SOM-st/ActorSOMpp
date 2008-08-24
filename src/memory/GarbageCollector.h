#pragma once

#ifndef GARBAGECOLLECTOR_H_
#define GARBAGECOLLECTOR_H_

class Heap;

class VMObject;

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
	

};

#endif