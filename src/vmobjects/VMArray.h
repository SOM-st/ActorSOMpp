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

	void        SetIndexableField(int idx, VMObject* item);
	VMObject*   GetIndexableField(int idx);
	int         GetNumberOfIndexableFields();
	VMArray*    CopyAndExtendWith(VMObject*);
	void        CopyIndexableFieldsTo(VMArray*);

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
};

#endif
