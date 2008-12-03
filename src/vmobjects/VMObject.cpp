#include "VMObject.h"
#include "VMClass.h"
#include "VMSymbol.h"
#include "VMFrame.h"
#include "VMInvokable.h"

//VMObject::VMObject()
//{
//    VMObject::VMObject(0);
//}

//clazz is the only field of VMObject so
const int VMObject::VMObjectNumberOfFields = 1; 

VMObject::VMObject( int numberOfFields ) {
    //fields = (VMObject**)&clazz;//fields + sizeof(VMObject**); 
    this->SetNumberOfFields(numberOfFields + VMObjectNumberOfFields);//+1 because of the clazz field
    gcfield = 0; 
	hash = (int32_t)this;
    //Object size is set by the heap
   
}


//VMObject::~VMObject() {}




void VMObject::SetNumberOfFields(int nof) {
    this->numberOfFields = nof;

    for (int i = 0; i < nof ; ++i) {
        this->SetField(i, Globals::NilObject());
    }
}




void VMObject::Send(StdString selector_string, VMObject** arguments, int argc) {
    VMSymbol* selector = _UNIVERSE->SymbolFor(selector_string);
    VMFrame* frame = _UNIVERSE->GetInterpreter()->GetFrame();
    frame->Push(this);

    for(int i = 0; i < argc; ++i) {
        frame->Push(arguments[i]);
    }

    VMClass* cl = this->GetClass();
    VMInvokable* invokable = dynamic_cast<VMInvokable*>(
                                            cl->LookupInvokable(selector));
    (*invokable)(frame);
}


int VMObject::GetDefaultNumberOfFields() const {
	return VMObjectNumberOfFields; 
}

VMClass* VMObject::GetClass() const {
	return clazz;
}

void VMObject::SetClass(VMClass* cl) {
	clazz = cl;
}

VMSymbol* VMObject::GetFieldName(int index) const {
    return this->clazz->GetInstanceFieldName(index);
}

int VMObject::GetFieldIndex(VMSymbol* fieldName) const {
    return this->clazz->LookupFieldIndex(fieldName);
}

int VMObject::GetNumberOfFields() const {
    return this->numberOfFields;
}

int VMObject::GetObjectSize() const {
    return objectSize;
}


bool VMObject::GetGCField() const {
    return gcfield;
}

	
void VMObject::SetGCField(bool value) { 
    gcfield = value; 
}


void VMObject::Assert(bool value) const {
    _UNIVERSE->Assert(value);
}


VMObject* VMObject::GetField(int index) const {
    return FIELDS[index]; 
}


void VMObject::SetField(int index, VMObject* value) {
     FIELDS[index] = value;
}

void VMObject::MarkReferences() {
    if (this->gcfield) return;
	this->SetGCField(1);
    for( int i = 0; i < this->GetNumberOfFields(); ++i) {
        FIELDS[i]->MarkReferences();
    }
//	clazz->MarkReferences();
}
