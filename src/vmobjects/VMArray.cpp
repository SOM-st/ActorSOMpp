#include "VMArray.h"

#include "../vm/Universe.h"


#define theEntries(i) FIELDS[this->GetNumberOfFields()+i]

const int VMArray::VMArrayNumberOfFields = 0; 

VMArray::VMArray(int size, int nof) : VMObject(nof + VMArrayNumberOfFields) {
    _HEAP->StartUninterruptableAllocation();
	
    for (int i = 0; i < size ; ++i) {
        (*this)[i] = Globals::NilObject();
        //this->SetIndexableField(i, Globals::NilObject());
    }
    _HEAP->EndUninterruptableAllocation();
	//objectSize += size * sizeof(pVMObject); 
    //calculate actual object size including the entries
    //done by the Heap on allocation (TODO: fix!)
}


pVMArray VMArray::CopyAndExtendWith(pVMObject item) const {
    size_t fields = GetNumberOfIndexableFields();
	pVMArray result = _UNIVERSE->NewArray(fields+1);
    this->CopyIndexableFieldsTo(result);
	(*result)[fields] = item; //->SetIndexableField(fields, item);
	return result;
}


pVMObject& VMArray::operator[](int idx) const {
    /*if (idx > GetNumberOfIndexableFields()-1 || idx < 0) {
        cout << "Array index out of bounds: Accessing " << idx 
             << ", but array size is only " << GetNumberOfIndexableFields()-1 
             << endl;
        _UNIVERSE->ErrorExit("Array index out of bounds exception");
        //throw std::bad_exception();
    }*/
    return theEntries(idx);
}


void VMArray::CopyIndexableFieldsTo(pVMArray to) const {
	for (int i = 0; i < this->GetNumberOfIndexableFields(); ++i) {
        (*to)[i] = (*this)[i];
	}
	
}

int VMArray::GetNumberOfIndexableFields() const {
    //cout << "(objSize - sizeof(VMArray))/4: " << (objectSize - sizeof(VMArray))/4 << endl;
    //cout << "size: " << size->GetEmbeddedInteger() << endl;
    /*if ((objectSize - (sizeof(VMObject)+sizeof(pVMObject)*(this->GetNumberOfFields()-1)))/4 != size->GetEmbeddedInteger()) {
        cout << "(objSize - sizeof(VMArray))/4: " << (objectSize - sizeof(VMArray))/4 << endl;
        cout << "size: " << size->GetEmbeddedInteger() << endl;
        cout << "(objectSize - sizeof(VMObject)*(this->GetNumberOfFields()-1))/4: "
             << (objectSize - (sizeof(VMObject)+sizeof(pVMObject)*(this->GetNumberOfFields()-1)))/4 << endl;
    }*/
    return this->GetAdditionalSpaceConsumption() / sizeof(pVMObject);//size->GetEmbeddedInteger();
}

void VMArray::MarkReferences() {
    if (gcfield) return;
    VMObject::MarkReferences();
//    this->size->MarkReferences();
	for (int i = 0 ; i < GetNumberOfIndexableFields() ; ++i) {
		if (theEntries(i) != NULL)
			theEntries(i)->MarkReferences();
	}
    
}
