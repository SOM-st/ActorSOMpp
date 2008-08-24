#pragma once

#ifndef GARBAGECOLLECTOR_H_
#define GARBAGECOLLECTOR_H_

class GarbageCollector
{
public:
	GarbageCollector();
	~GarbageCollector();
	void Collect();

	

private:
	void markReachableObjects();
	void markObject();
	void mergeFreeSpaces();

	

};

#endif