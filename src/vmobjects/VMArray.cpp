#include "VMArray.h"

#include "../vm/Universe.h"

#define theEntries(i) fields[this->GetNumberOfFields()+i]

VMArray::VMArray(int size, int nof) : VMObject(nof+2)
{
    //this->SetNumberOfFields(this->GetNumberOfFields() + 2);
	this->size = _UNIVERSE->new_integer(size);
	this->entries = _UNIVERSE->new_integer(0);
	objectSize += size * sizeof(VMObject*); //calculate actual object size including the entries
	//VMObject** t = theEntries;
}



//VMArray::~VMArray()
//{
//}
//
//void VMArray::ResetEntriesPointer()
//{
//    //theEntries = theEntries;(VMObject**)&theEntries + this->GetOffset();
//}
//
//size_t VMArray::GetOffset()
//{
//    //return sizeof(VMInteger*)*2;
//    return 0;
//}

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
	/*for (int i = 0; i < size; ++i)
	{
		VMObject* item = theEntries[i];
		if (item != 0){
			if (idx == 0) return item;
			--idx;
		}
	}
	return 0;*/
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
	return entries->GetEmbeddedInteger();
}

/*int VMArray::SetIndexableField(VMObject* item)
{
    if (size->GetEmbeddedInteger() <= entries->GetEmbeddedInteger()) {
        cout << "Array index out of bounds: Accessing " << size->GetEmbeddedInteger()+1 << ", but there is only space for " << size->GetEmbeddedInteger();
        cout << " entries available\n";
        _UNIVERSE->error_exit("Array index out of bounds exception");
        //throw std::bad_exception();
    }
	for (int i = 0; i < size->GetEmbeddedInteger(); ++i)
	{
		if (theEntries(i) == NULL)
		{
			theEntries(i) = item;
            int32_t tmp =  entries->GetEmbeddedInteger();
			entries->SetEmbeddedInteger(++tmp);
			return i;
        }
	}
	return -1;
}*/

void VMArray::SetIndexableField(int idx, VMObject* item)
{
	if (idx > size->GetEmbeddedInteger()-1 || idx < 0) {
        cout << "Array index out of bounds: Accessing " << idx << ", but there is only space for " << size->GetEmbeddedInteger();
        cout << " entries available\n";
        _UNIVERSE->error_exit("Array index out of bounds exception");
        //throw std::bad_exception();
    }
    int32_t tmp = entries->GetEmbeddedInteger();
	if (idx >= tmp && item != 0) entries->SetEmbeddedInteger(tmp + 1);
	else if (item == 0) entries->SetEmbeddedInteger(tmp - 1);
    //else: entry is replaced, number of entries does not change
	theEntries(idx) = item;
}

int VMArray::GetArraySize()
{
	return size->GetEmbeddedInteger();
}

//VMObject* VMArray::GetItem(int idx)
//{
//	if (idx >= size) throw std::bad_exception();
//	return theEntries[idx];
//}

void VMArray::MarkReferences()
{
    VMObject::MarkReferences();
    this->size->MarkReferences();
    this->entries->MarkReferences();
	for (int i = 0 ; i < size->GetEmbeddedInteger() ; ++i)
	{
		if (theEntries(i) != NULL)
			theEntries(i)->MarkReferences();
	}
}
