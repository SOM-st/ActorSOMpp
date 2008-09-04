#include "VMPrimitive.h"
#include "VMSymbol.h"
#include "VMClass.h"
#include "../vm/Universe.h"

VMPrimitive* VMPrimitive::GetEmptyPrimitive( VMSymbol* sig )
{
    VMPrimitive* prim = new (_HEAP) VMPrimitive(sig);
    prim->empty = true;
    prim->SetRoutine(new Routine(&VMPrimitive::empty_routine));
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
}

bool VMPrimitive::IsEmpty()
{
    return empty;
}

void VMPrimitive::SetRoutine(Routine* rtn)
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
}

void VMPrimitive::empty_routine( VMObject* _self, VMFrame* frame )
{
    VMInvokable* self = (VMInvokable*) _self;
    VMSymbol* sig = self->get_signature();
    cout << "undefined primitive called: " << sig->GetChars() << endl;
}

