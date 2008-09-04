#include "VMArray.h"
#include "../vm/Universe.h"

VMArray::VMArray(int size)
{
	this->size = size;
	this->entries = 0;
	objectSize = sizeof(VMArray) + size * sizeof(VMObject*);
	theEntries = (VMObject**)&theEntries + this->GetOffset();
}

VMArray::~VMArray()
{
}

void VMArray::ResetEntriesPointer()
{
    theEntries = (VMObject**)&theEntries + this->GetOffset();
}

size_t VMArray::GetOffset()
{
    return sizeof(VMObject**);
}

VMArray* VMArray::CopyAndExtendWith(VMObject* item)
{
	VMArray* result = new (_HEAP, this->entries+1) VMArray(this->entries+1);
	int j = 0;
	for (int i = 0; i < this->size; ++i)
	{
		if (this->theEntries[i] != 0){
			result->SetIndexableField(j, theEntries[i]);
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
    if (idx >= size) throw std::bad_exception();
    return theEntries[idx];
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
	return entries;
}

int VMArray::SetIndexableField(VMObject* item)
{
	if (size <= entries) throw std::bad_exception();
	for (int i = 0; i < size; ++i)
	{
		if (theEntries[i] == 0)
		{
			theEntries[i] = item;
			entries++;
			return i;
		}
	}
	return -1;
}

void VMArray::SetIndexableField(int idx, VMObject* item)
{
	if (idx >= size) throw std::bad_exception();
	if (theEntries[idx] == 0 && item != 0) entries++;
	else if (item == 0) entries--;
	theEntries[idx] = item;
}

int VMArray::GetArraySize()
{
	return size;
}

//VMObject* VMArray::GetItem(int idx)
//{
//	if (idx >= size) throw std::bad_exception();
//	return theEntries[idx];
//}

void VMArray::MarkReferences()
{
	this->setGCField(1);
	for (int i = 0 ; i < size ; ++i)
	{
		if (theEntries[i] != NULL)
			theEntries[i]->MarkReferences();
	}
}