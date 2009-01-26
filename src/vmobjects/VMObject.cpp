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
    //fields = (pVMObject*)&clazz;//fields + sizeof(pVMObject*); 
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




void VMObject::Send(StdString selector_string, pVMObject* arguments, int argc) {
    pVMSymbol selector = _UNIVERSE->SymbolFor(selector_string);
    pVMFrame frame = _UNIVERSE->GetInterpreter()->GetFrame();
    frame->Push(this);

    for(int i = 0; i < argc; ++i) {
        frame->Push(arguments[i]);
    }

    pVMClass cl = this->GetClass();
    pVMInvokable invokable = dynamic_cast<pVMInvokable>(
                                            cl->LookupInvokable(selector));
    (*invokable)(frame);
}


int VMObject::GetDefaultNumberOfFields() const {
	return VMObjectNumberOfFields; 
}

pVMClass VMObject::GetClass() const {
	return clazz;
}

void VMObject::SetClass(pVMClass cl) {
	clazz = cl;
}

pVMSymbol VMObject::GetFieldName(int index) const {
    return this->clazz->GetInstanceFieldName(index);
}

int VMObject::GetFieldIndex(pVMSymbol fieldName) const {
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


pVMObject VMObject::GetField(int index) const {
    return FIELDS[index]; 
}


void VMObject::SetField(int index, pVMObject value) {
     FIELDS[index] = value;
}

//returns the Object's additional memory used (e.g. for Array fields)
int VMObject::GetAdditionalSpaceConsumption() const
{
    //The VM*-Object's additional memory used needs to be calculated.
    //It's      the total object size   MINUS   sizeof(VMObject) for basic
    //VMObject  MINUS   the number of fields times sizeof(pVMObject)
    return (objectSize - (sizeof(VMObject) + 
                          sizeof(pVMObject) * (this->GetNumberOfFields() - 1)));
}


void VMObject::MarkReferences() {
    if (this->gcfield) return;
	this->SetGCField(1);
    for( int i = 0; i < this->GetNumberOfFields(); ++i) {
        FIELDS[i]->MarkReferences();
    }
//	clazz->MarkReferences();
}
