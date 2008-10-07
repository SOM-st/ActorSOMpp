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
    VMArray(int size, int nof = 0);
	//virtual ~VMArray();
	virtual void MarkReferences();
	void SetIndexableField(int idx, VMObject* item);
	//VMObject* GetItem(int idx);
	VMObject* GetIndexableField(int idx);
	int GetNumberOfIndexableFields();
	VMArray* CopyAndExtendWith(VMObject*);
	void CopyIndexableFieldsTo(VMArray*);
    //virtual size_t GetOffset();

	VMObject* operator[](int idx)
	{
		if (idx > size->GetEmbeddedInteger() - 1) {
            cout << "Array index out of bounds: Accessing " << idx << ", but there is only space for " << size->GetEmbeddedInteger();
            cout << " entries available\n";
            _UNIVERSE->error_exit("Array index out of bounds exception");
            //throw std::bad_exception();
        }
		return GetIndexableField(idx);
	}

private:
	VMInteger* size;
	//VMInteger* entries;
	//VMObject** theEntries;
	//void assertType(VMObject& vmo) const {}; //method to assert that only VMObject class is supported
	//std::vector<T> theEntries;

};

#endif
