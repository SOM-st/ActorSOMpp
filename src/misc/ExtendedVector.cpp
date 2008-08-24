
#include "ExtendedVector.h"
#include <iterator>

ExtendedVector::ExtendedVector()
{
	theVector.clear();
}

void ExtendedVector::Add(VMObject *ptr)
{
	theVector.push_back(ptr);
}

void ExtendedVector::AddIfAbsent(VMObject *ptr)
{
	if (IndexOf(ptr) == -1) Add(ptr);
}

VMObject* ExtendedVector::Get(int index)
{
	return theVector.at(index);
}

int ExtendedVector::Size()
{
	return theVector.size();
}

int ExtendedVector::IndexOf(VMObject *needle) {
	for (vector<VMObject*>::iterator it = theVector.begin(); it!=theVector.end(); ++it) {
		if (*it == needle) return distance(theVector.begin(), it);
		//cout << *it << endl;
	}
	return -1;
}