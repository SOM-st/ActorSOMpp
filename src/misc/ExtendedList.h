#pragma once
#ifndef EXTENDEDLIST_H_
#define EXTENDEDLIST_H_

#include <list>
#include <memory>
#include "../vmobjects/VMObject.h"

class ExtendedList{
public:
	ExtendedList();
	~ExtendedList();
	
	void Add(VMObject* ptr);
	void AddIfAbsent(VMObject* ptr);
	void AddAll(ExtendedList* list);
	void Clear();
	int Size();
	VMObject* get(int index);
	int IndexOf(VMObject* needle);

private:
	std::list<VMObject*> theList;
};

#endif
