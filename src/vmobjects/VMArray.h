#pragma once

#ifndef VMARRAY_H_
#define VMARRAY_H_

//#include <vector>
#include "VMObject.h"
#include "VMInteger.h"

class VMArray : public VMObject {
public:
    VMArray(int size, int nof = 0);
	//virtual ~VMArray();
	virtual void MarkReferences();

	void        SetIndexableField(int idx, VMObject* item);
	
	int         GetNumberOfIndexableFields();
	VMArray*    CopyAndExtendWith(VMObject*);
	void        CopyIndexableFieldsTo(VMArray*);

	VMObject* operator[](int idx) {
		return GetIndexableField(idx);
	}

private:
    VMObject*   GetIndexableField(int idx);

	VMInteger* size;
};

#endif
