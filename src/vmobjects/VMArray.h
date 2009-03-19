#pragma once

#ifndef VMARRAY_H_
#define VMARRAY_H_

#include "VMObject.h"
#include "VMInteger.h"

class VMArray : public VMObject {
public:
    VMArray(int size, int nof = 0);
	//virtual ~VMArray();
	virtual void MarkReferences();

	virtual int         GetNumberOfIndexableFields() const;
	pVMArray    CopyAndExtendWith(pVMObject) const;
	void        CopyIndexableFieldsTo(pVMArray) const;

	pVMObject& operator[](int idx) const;

private:
    static const int VMArrayNumberOfFields;
};


#endif
