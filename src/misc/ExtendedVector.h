#pragma once
#ifndef EXTENDEDVECTOR_H_

#include <vector>
#include "../vmobjects/OOObject.h"

class ExtendedVector{
public:
	ExtendedVector();
	void Add(OOObject *ptr);
	void AddIfAbsent(OOObject *ptr);
	void Clear();
	int Size();
	int IndexOf(OOObject *needle);
	OOObject* Get(int index);
private:
	vector<OOObject*> theVector;
	
};

#endif
