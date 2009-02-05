#include "VMPrimitive.h"
#include "VMSymbol.h"
#include "VMClass.h"

#include "../vm/Universe.h"

//needed to instanciate the Routine object for the  empty routine
#include "../primitivesCore/Routine.h"


pVMPrimitive VMPrimitive::GetEmptyPrimitive( pVMSymbol sig ) {
    
    pVMPrimitive prim = new (_HEAP) VMPrimitive(sig);
    prim->empty = (bool*)1;
    prim->SetRoutine(new Routine<VMPrimitive>(prim, &VMPrimitive::EmptyRoutine));
    return prim;
}

const int VMPrimitive::VMPrimitiveNumberOfFields = 2; 

VMPrimitive::VMPrimitive(pVMSymbol signature) : VMInvokable(VMPrimitiveNumberOfFields) {
    _HEAP->StartUninterruptableAllocation();
    //the only class that explicitly does this.
    this->SetClass(Globals::PrimitiveClass());
    //this->empty = (bool*)_HEAP->Allocate(sizeof(bool));
    this->SetSignature(signature);
    this->routine = NULL;
    this->empty = (bool*)0;

    _HEAP->EndUninterruptableAllocation();
}


//TODO: this is a memory leak. The empty flag is not freed and the routine is not destroyed
//, but G++ doesn't like the destructor of VMObject classes
//VMPrimitive::~VMPrimitive(Heap* heap)
//{
//    _HEAP->Free(empty, sizeof(bool));
//    if (routine != NULL) Core::destroy(routine);
//}




void VMPrimitive::MarkReferences() {
    if (gcfield) return;
    //VMInvokable::MarkReferences();
    this->SetGCField(1);
    for( int i = 0; i < this->GetNumberOfFields(); ++i) {
        //HACK to avoid calling MarkReferences() for the bool*
        if ((void*)FIELDS[i] == (void*)this->empty) 
            continue;
        //HACK to avoid calling MarkReferences() for the routine*
        if ((void*)FIELDS[i] == (void*)this->routine)
            continue;//cout << "routine" << endl;
        FIELDS[i]->MarkReferences();
    }
}


void VMPrimitive::EmptyRoutine( pVMObject _self, pVMFrame /*frame*/ ) {
    pVMInvokable self = (pVMInvokable)( _self );
    pVMSymbol sig = self->GetSignature();
    cout << "undefined primitive called: " << sig->GetChars() << endl;
}


