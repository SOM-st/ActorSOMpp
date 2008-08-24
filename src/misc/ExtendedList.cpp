
#include "ExtendedList.h"
#include <iterator>

ExtendedList::ExtendedList()
{
	theList.clear();
}

ExtendedList::~ExtendedList()
{
}

void ExtendedList::Add(VMObject *ptr)
{
	theList.push_back(ptr);
}

void ExtendedList::AddAll(ExtendedList *list)
{
	theList.merge(list->theList);
}

void ExtendedList::AddIfAbsent(VMObject *ptr)
{
	if (IndexOf(ptr) == -1) Add(ptr);
}

void ExtendedList::Clear()
{
	theList.clear();
}

VMObject* ExtendedList::get(int index)
{
	for (list<VMObject*>::iterator it = theList.begin(); it!=theList.end(); ++it){
		if (index == 0) return *it;
		--index;
	}
	return NULL;
}

int ExtendedList::Size()
{
	return theList.size();
}

int ExtendedList::IndexOf(VMObject* needle) {
	for (list<VMObject*>::iterator it = theList.begin(); it!=theList.end(); ++it) {
		if (*it == needle) return distance(theList.begin(), it);
		//cout << *it << endl;
	}
	return -1;
}