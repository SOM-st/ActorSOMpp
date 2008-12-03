#include "VMArray.h"

#include "../vm/Universe.h"


#define theEntries(i) FIELDS[this->GetNumberOfFields()+i]


VMArray::VMArray(int size, int nof) : VMObject(nof+1) {
    _HEAP->StartUninterruptableAllocation();
	this->size = _UNIVERSE->NewInteger(size);

    for (int i = 0; i < size ; ++i) {
        (*this)[i] = Globals::NilObject();
        //this->SetIndexableField(i, Globals::NilObject());
    }
    _HEAP->EndUninterruptableAllocation();
	//objectSize += size * sizeof(VMObject*); 
    //calculate actual object size including the entries
    //done by the Heap on allocation (TODO: fix!)
}


VMArray* VMArray::CopyAndExtendWith(VMObject* item) const {
    size_t fields = this->size->GetEmbeddedInteger();
	VMArray* result = _UNIVERSE->NewArray(fields+1);
    this->CopyIndexableFieldsTo(result);
	(*result)[fields] = item; //->SetIndexableField(fields, item);
	return result;
}


VMObject*& VMArray::operator[](int idx) const {
    if (idx > size->GetEmbeddedInteger()-1 || idx < 0) {
        cout << "Array index out of bounds: Accessing " << idx 
             << ", but array size is only " << size->GetEmbeddedInteger()-1 
             << endl;
        _UNIVERSE->ErrorExit("Array index out of bounds exception");
        //throw std::bad_exception();
    }
    return theEntries(idx);
}


void VMArray::CopyIndexableFieldsTo(VMArray* to) const {
	for (int i = 0; i < this->GetNumberOfIndexableFields(); ++i) {
        (*to)[i] = (*this)[i];
	}
	
}

int VMArray::GetNumberOfIndexableFields() const {
    //cout << "(objSize - sizeof(VMArray))/4: " << (objectSize - sizeof(VMArray))/4 << endl;
    //cout << "size: " << size->GetEmbeddedInteger() << endl;
    /*if ((objectSize - sizeof(VMArray))/4 != size->GetEmbeddedInteger()) {
        cout << "(objSize - sizeof(VMArray))/4: " << (objectSize - sizeof(VMArray))/4 << endl;
        cout << "size: " << size->GetEmbeddedInteger() << endl;
    }*/
    return size->GetEmbeddedInteger();
}

void VMArray::MarkReferences() {
    if (gcfield) return;
    VMObject::MarkReferences();
//    this->size->MarkReferences();
	for (int i = 0 ; i < size->GetEmbeddedInteger() ; ++i) {
		if (theEntries(i) != NULL)
			theEntries(i)->MarkReferences();
	}
    
}
