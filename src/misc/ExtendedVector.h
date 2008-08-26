#pragma once
#ifndef EXTENDEDVECTOR_H_

#include <vector>
#include "../vmobjects/VMObject.h"

class ExtendedVector{
public:
	ExtendedVector();
	void Add(VMObject *ptr);
	void AddIfAbsent(VMObject *ptr);
	void Clear();
	int Size();
	int IndexOf(VMObject *needle);
	VMObject* Get(int index);

	VMObject* operator[](int key) 
	{
		return theVector[key];
	}

private:
	vector<VMObject*> theVector;
	
};

#endif
