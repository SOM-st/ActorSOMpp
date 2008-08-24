#include "VMObject.h"

VMObject::VMObject() : OOObject()
{
}

VMObject::~VMObject() {}

pString VMObject::GetClass()
{
	return "";
}

void VMObject::SetClass(pString className)
{
}

pString VMObject::GetFieldName(int index)
{
	return "";
}

int VMObject::GetFieldIndex(pString fieldName)
{
	return 0;
}

int VMObject::GetNumberOfFields()
{
	return 0;
}

void VMObject::SetNumberOfFields(int nof)
{
}

int VMObject::GetDefaultNumberOfFields()
{
	return 0;
}

void VMObject::Send(pString, VMObject*, int)
{
}

VMObject* VMObject::GetField(int index)
{
	return NULL;
}

void VMObject::SetField(int index, VMObject* value)
{
}

void VMObject::MarkReferences()
{
}
