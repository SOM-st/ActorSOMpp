#include "VMArray.h"

#include "../vm/Universe.h"

#define theEntries(i) FIELDS[this->GetNumberOfFields()+i]

VMArray::VMArray(int size, int nof) : VMObject(nof+1)
{
	this->size = _UNIVERSE->new_integer(size);

    for (int i = 0; i < size ; ++i)
    {
        this->SetIndexableField(i, nil_object);
    }
	//objectSize += size * sizeof(VMObject*); //calculate actual object size including the entries
}

VMArray* VMArray::CopyAndExtendWith(VMObject* item)
{
    size_t fields = this->size->GetEmbeddedInteger();
	VMArray* result = _UNIVERSE->new_array(fields+1);
    this->CopyIndexableFieldsTo(result);
	result->SetIndexableField(fields, item);
	return result;
}

VMObject* VMArray::GetIndexableField(int idx)
{
    if (idx > size->GetEmbeddedInteger()-1 || idx < 0)  {
        cout << "Array index out of bounds: Accessing " << idx << ", but only " << size->GetEmbeddedInteger()-1;
        cout << " entries are available\n";
        _UNIVERSE->error_exit("Array index out of bounds exception");
        //throw std::bad_exception();
    }
    return theEntries(idx);
}

void VMArray::CopyIndexableFieldsTo(VMArray* to)
{
	for (int i = 0; i < this->GetNumberOfIndexableFields(); ++i)
	{
        to->SetIndexableField(i, this->GetIndexableField(i));
	}
	
}

int VMArray::GetNumberOfIndexableFields()
{
    return size->GetEmbeddedInteger();
}


void VMArray::SetIndexableField(int idx, VMObject* item)
{
	if (idx > size->GetEmbeddedInteger()-1 || idx < 0) {
        cout << "Array index out of bounds: Accessing " << idx << ", but there is only space for " << size->GetEmbeddedInteger();
        cout << " entries available\n";
        _UNIVERSE->error_exit("Array index out of bounds exception");
        //throw std::bad_exception();
    }
   	theEntries(idx) = item;
}

void VMArray::MarkReferences()
{
    
//    this->size->MarkReferences();
	for (int i = 0 ; i < size->GetEmbeddedInteger() ; ++i)
	{
		if (theEntries(i) != NULL)
			theEntries(i)->MarkReferences();
	}
    VMObject::MarkReferences();
}
