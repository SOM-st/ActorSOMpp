#include "VMObject.h"
#include "VMClass.h"
#include "VMSymbol.h"
#include "VMFrame.h"
#include "VMInvokable.h"

//VMObject::VMObject()
//{
//    VMObject::VMObject(0);
//}

VMObject::VMObject( int number_of_fields )
{
    fields = (VMObject**)&clazz;//fields + sizeof(VMObject**); 
    this->SetNumberOfFields(number_of_fields+1); //CSOM calculates VMObject::clazz's size out here. why????
    gcfield = 0; 
	hash = (int32_t)this;
	objectSize = sizeof(VMObject) + number_of_fields*sizeof(VMObject*);//sizeof(VMObject) includes the space for the clazz field
}

//VMObject::~VMObject() {}

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
    return this->clazz->get_instance_field_name(index);
}

int VMObject::GetFieldIndex(VMSymbol* fieldName)
{
    return this->clazz->lookup_field_index(fieldName);
}

int VMObject::GetNumberOfFields()
{
    return this->numberOfFields;
}

void VMObject::SetNumberOfFields(int nof)
{
    this->numberOfFields = nof;

    for (int i = 0; i < nof ; ++i)
    {
        this->SetField(i, nil_object);
    }
}

int VMObject::GetDefaultNumberOfFields()
{
	return this->numberOfFields - 1; 
}

void VMObject::Send(pString selector_string, VMObject** arguments, int argc)
{
    VMSymbol* selector = _UNIVERSE->symbol_for(selector_string);
    VMFrame* frame = _UNIVERSE->GetInterpreter()->GetFrame();
    frame->Push(this);

    for(int i = 0; i < argc; ++i)
    {
        frame->Push(arguments[i]);
    }

    VMClass* cl = this->GetClass();
    VMInvokable* invokable = dynamic_cast<VMInvokable*>(cl->lookup_invokable(selector));
    invokable->invoke(frame);
}

void VMObject::Assert(bool value)
{
    _UNIVERSE->Assert(value);
}

VMObject* VMObject::GetField(int index)
{
    return this->fields[index]; 
}

void VMObject::SetField(int index, VMObject* value)
{
    this->fields[index] = value;
}

void VMObject::MarkReferences()
{
	this->setGCField(1);
	clazz->MarkReferences();
}
