#pragma once
#ifndef OOOBJECT_H_

#include "../misc/defs.h"
#include "../memory/Heap.h"

class OOObject {

public:
	OOObject() { gcfield = 0; hash = (uint16_t)this; };
	virtual ~OOObject() {};
	int32_t getGCField() {return gcfield;} ;
	void setGCField(int32_t value) { gcfield = value; } ;
	void *operator new( unsigned int num_bytes, Heap *heap)
	{
		return heap->Allocate(num_bytes);
	}

	 void operator delete( void *self, Heap *heap ) 
	 {
		 heap->Free(self); 
	 } 

	 uint16_t operator()(const std::string &str) const
	{
		uint16_t sum=0;
		for (int i=0; i<str.size(); ++i)
			sum=(sum+str[i])%100;
		return sum;

	}

private:
	int32_t gcfield;
	uint16_t hash;
};


#endif