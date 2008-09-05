#include "VMPrimitive.h"
#include "VMSymbol.h"
#include "VMClass.h"
#include "../vm/Universe.h"

//needed to instanciate the Routine object for the  empty routine
#include "../primitives/Routine.h"

VMPrimitive* VMPrimitive::GetEmptyPrimitive( VMSymbol* sig )
{
    VMPrimitive* prim = new (_HEAP) VMPrimitive(sig);
    prim->empty = true;
    prim->SetRoutine(new (_HEAP) Routine<VMPrimitive>(prim, &VMPrimitive::empty_routine));
    return prim;
}

VMPrimitive::VMPrimitive(VMSymbol* signature) : VMObject(), VMInvokable()
{
    this->SetClass(primitive_class);

    this->set_signature(signature);
    this->routine = NULL;
    this->empty = false;
}

VMPrimitive::~VMPrimitive()
{
    if (routine != NULL) Core::destroy(routine);
}

bool VMPrimitive::IsEmpty()
{
    return empty;
}

void VMPrimitive::SetRoutine(PrimitiveRoutine* rtn)
{
    routine = rtn;
}

void VMPrimitive::invoke(VMFrame *frm)
{
    (*routine)(this, frm);
}

void VMPrimitive::MarkReferences()
{
    VMObject::MarkReferences();
    signature->MarkReferences();
    holder->MarkReferences();
   // routine->MarkReferences();
}

void VMPrimitive::empty_routine( VMObject* _self, VMFrame* frame )
{
    VMInvokable* self = (VMInvokable*) _self;
    VMSymbol* sig = self->get_signature();
    cout << "undefined primitive called: " << sig->GetChars() << endl;
}

