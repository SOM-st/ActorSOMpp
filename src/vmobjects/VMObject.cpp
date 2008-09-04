#include "VMObject.h"
#include "VMClass.h"

VMObject::VMObject()
{
	VMObject(0);
}

VMObject::VMObject( int number_of_fields )
{
    gcfield = 0; 
	hash = (uint16_t)this;
	objectSize = sizeof(VMObject) + number_of_fields*sizeof(VMObject*);
    numberOfFields = number_of_fields;
    fields = (VMObject**)&this->clazz;
}



VMObject::~VMObject() {}

VMClass* VMObject::GetClass()
{
	return clazz;
}

void VMObject::SetClass(VMClass* cl)
{
	clazz = cl;
}

VMSymbol* VMObject::GetFieldName(int index)
{
	return NULL;
}

int VMObject::GetFieldIndex(VMSymbol* fieldName)
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
	this->setGCField(1);
	clazz->MarkReferences();
}
