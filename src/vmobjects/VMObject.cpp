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
    //fields = (VMObject**)&clazz;//fields + sizeof(VMObject**); 

    this->SetNumberOfFields(number_of_fields+1);//+1 because of the clazz field
    gcfield = 0; 
	hash = (int32_t)this;
    //cout << sizeof(VMObject) << endl;
    //objectSize = sizeof(VMObject) + number_of_fields*sizeof(VMObject*);//sizeof(VMObject) includes the space for the clazz field
   
}

//VMObject::~VMObject() {}

VMClass* VMObject::GetClass()
{
	return clazz;
}

void VMObject::SetClass(VMClass* cl)
{
	clazz = cl;
    if (cl->GetName()->GetStdString() == pString("Hashtable")){
        cout << "hier";
    }
}

VMSymbol* VMObject::GetFieldName(int index)
{
    return this->clazz->GetInstanceFieldName(index);
}

int VMObject::GetFieldIndex(VMSymbol* fieldName)
{
    return this->clazz->LookupFieldIndex(fieldName);
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
    VMInvokable* invokable = dynamic_cast<VMInvokable*>(cl->LookupInvokable(selector));
    invokable->Invoke(frame);
}

int VMObject::GetObjectSize() 
{
    return objectSize;
}

bool VMObject::GetGCField() 
{
    return gcfield;
}
	
void VMObject::SetGCField(bool value) 
{ 
    gcfield = value; 
}

void VMObject::Assert(bool value)
{
    _UNIVERSE->Assert(value);
}

VMObject* VMObject::GetField(int index)
{
    return FIELDS[index]; 
}

void VMObject::SetField(int index, VMObject* value)
{
    if (this->GetClass() != NULL && this->GetClass() != nil_object)
        if (this->GetClass()->GetName() != NULL && this->GetClass()->GetName() != nil_object)
            if (this->GetClass()->GetName()->GetStdString() == pString("Hashtable"))
            {
                cout << "hier";
            }
     FIELDS[index] = value;
}

void VMObject::MarkReferences()
{
    if (this->gcfield) return;
	this->SetGCField(1);
    for( int i = 0; i < this->GetNumberOfFields(); ++i)
    {
        FIELDS[i]->MarkReferences();
    }
//	clazz->MarkReferences();
}
