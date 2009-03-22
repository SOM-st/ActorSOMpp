#include "VMArray.h"

#include "../vm/Universe.h"


#define theEntries(i) FIELDS[this->GetNumberOfFields()+i]

const int VMArray::VMArrayNumberOfFields = 0; 

VMArray::VMArray(int size, int nof) : VMObject(nof + VMArrayNumberOfFields) {
    _HEAP->StartUninterruptableAllocation();
	
    for (int i = 0; i < size ; ++i) {
        (*this)[i] = nilObject;
    }
    _HEAP->EndUninterruptableAllocation();

}


pVMArray VMArray::CopyAndExtendWith(pVMObject item) const {
    size_t fields = GetNumberOfIndexableFields();
	pVMArray result = _UNIVERSE->NewArray(fields+1);
    this->CopyIndexableFieldsTo(result);
	(*result)[fields] = item;
	return result;
}


pVMObject& VMArray::operator[](int idx) const {
    if (idx > GetNumberOfIndexableFields()-1 || idx < 0) {
        cout << "Array index out of bounds: Accessing " << idx 
             << ", but array size is only " << GetNumberOfIndexableFields()-1 
             << endl;
        _UNIVERSE->ErrorExit("Array index out of bounds");
    }
    return theEntries(idx);
}


void VMArray::CopyIndexableFieldsTo(pVMArray to) const {
	for (int i = 0; i < this->GetNumberOfIndexableFields(); ++i) {
        (*to)[i] = (*this)[i];
	}
	
}

int VMArray::GetNumberOfIndexableFields() const {
    return this->GetAdditionalSpaceConsumption() / sizeof(pVMObject);
}

void VMArray::MarkReferences() {
    if (gcfield) return;
    VMObject::MarkReferences();
	for (int i = 0 ; i < GetNumberOfIndexableFields() ; ++i) {
		if (theEntries(i) != NULL)
			theEntries(i)->MarkReferences();
	}
    
}
