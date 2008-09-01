#include "VMPrimitive.h"
#include "VMSymbol.h"
#include "VMClass.h"

VMPrimitive::VMPrimitive() : VMObject()
{
}

VMPrimitive::VMPrimitive(MethodGenerationContext* mgenc)
{
}

VMPrimitive::~VMPrimitive()
{
}

bool VMPrimitive::IsEmpty()
{
    return empty;
}

void VMPrimitive::SetRoutine(Routine<Primitive>* rtn)
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