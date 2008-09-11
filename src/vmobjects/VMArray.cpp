#include "VMArray.h"

#include "../vm/Universe.h"

#define theEntries(i) fields[this->GetNumberOfFields()+1+i]

VMArray::VMArray(int size, int nof) : VMObject(nof+2)
{
    //this->SetNumberOfFields(this->GetNumberOfFields() + 2);
	this->size = _UNIVERSE->new_integer(size);
	this->entries = _UNIVERSE->new_integer(0);
	objectSize += size * sizeof(VMObject*); //calculate actual object size including the entries
	//VMObject** t = theEntries;
}



VMArray::~VMArray()
{
}
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

	VMArray* result = new (_HEAP, this->entries->GetEmbeddedInteger()+1) VMArray(this->entries->GetEmbeddedInteger()+1);
	int j = 0;
	for (int i = 0; i < this->size->GetEmbeddedInteger(); ++i)
	{
		if (theEntries(i) != NULL){
			result->SetIndexableField(j, theEntries(i));
			++j;
		}
	}
	result->SetIndexableField(j, item);
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
    if (idx > entries->GetEmbeddedInteger() || idx < 0) throw std::bad_exception();
    return theEntries(idx);
}

void VMArray::CopyIndexableFieldsFrom(VMArray* from)
{
	if (this->size < this->entries + from->GetNumberOfIndexableFields()) {
	} else {
		for (int i = 0; i < from->GetNumberOfIndexableFields(); ++i)
		{
            SetIndexableField(i, from->GetIndexableField(i));
		}
	}
}

int VMArray::GetNumberOfIndexableFields()
{
	return entries->GetEmbeddedInteger();
}

int VMArray::SetIndexableField(VMObject* item)
{
	if (size <= entries) throw std::bad_exception();
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
}

void VMArray::SetIndexableField(int idx, VMObject* item)
{
	if (idx >= size->GetEmbeddedInteger()) throw std::bad_exception();
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