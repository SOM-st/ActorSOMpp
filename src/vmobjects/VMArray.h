#pragma once

#ifndef VMARRAY_H_
#define VMARRAY_H_
//#include <vector>
#include "VMObject.h"
#include "VMInteger.h"
//template <class T>
class VMArray : public VMObject
{
public:
	VMArray(int size);
	virtual ~VMArray();
	virtual void MarkReferences();
	void SetIndexableField(int idx, VMObject* item);
	int SetIndexableField(VMObject* item);
	//VMObject* GetItem(int idx);
	VMObject* GetIndexableField(int idx);
	int GetArraySize();
	int GetNumberOfIndexableFields();
	VMArray* CopyAndExtendWith(VMObject*);
	void CopyIndexableFieldsFrom(VMArray*);
    virtual size_t GetOffset();

	VMObject* operator[](int idx)
	{
		if (idx >= size->GetEmbeddedInteger()) throw std::bad_exception();
		return theEntries[idx];
	}

protected:
    virtual void ResetEntriesPointer();
private:
	VMInteger* size;
	VMInteger* entries;
	VMObject** theEntries;
	//void assertType(VMObject& vmo) const {}; //method to assert that only VMObject class is supported
	//std::vector<T> theEntries;

};

#endif