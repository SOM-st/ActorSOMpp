#pragma once
#ifndef EXTENDEDLIST_H_
#define EXTENDEDLIST_H_

#include <list>
#include <memory>
#include "../vmobjects/OOObject.h"

class ExtendedList{
public:
	ExtendedList();
	~ExtendedList();
	
	void Add(OOObject* ptr);
	void AddIfAbsent(OOObject* ptr);
	void AddAll(ExtendedList* list);
	void Clear();
	int Size();
	OOObject* get(int index);
	int IndexOf(OOObject* needle);

private:
	std::list<OOObject*> theList;
};

#endif
