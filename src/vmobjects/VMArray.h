#pragma once

#ifndef VMARRAY_H_
#define VMARRAY_H_
//#include <vector>
#include "VMObject.h"

//template <class T>
class VMArray : public VMObject
{
public:
	VMArray(int size);
	virtual ~VMArray();
	virtual void MarkReferences();
	void AddItem(int idx, VMObject* item);
	int AddItem(VMObject* item);
	VMObject* GetItem(int idx);
	VMObject* GetIndexableField(int idx);
	int GetArraySize();
	int GetNumberOfIndexableFields();
	VMArray* CopyAndExtendWith(VMObject*);
	void CopyIndexableFieldsFrom(VMArray*);

	VMObject* operator[](int idx)
	{
		if (idx >= size) throw std::bad_exception();
		return theEntries[idx];
	}

	void *operator new( size_t num_bytes, Heap *heap, int size)
	{
		return heap->Allocate(num_bytes + (size*sizeof(VMObject*)));
	}
	void operator delete( void *self, Heap *heap, int size) 
	 {
		 heap->Free(self); 
	 } 

private:
	int size;
	int entries;
	VMObject** theEntries;
	//void assertType(VMObject& vmo) const {}; //method to assert that only VMObject class is supported
	//std::vector<T> theEntries;

};

#endif