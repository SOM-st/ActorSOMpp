#pragma once

#ifndef VMPRIMITVE_H_
#define VMPRIMITVE_H_
#include "VMObject.h"
#include "Routine.h"
#include "../compiler/MethodGenerationContext.h"
#include "../primitives/Primitive.h"
#include "VMInvokable.h"

class VMPrimitive : public VMObject, public VMInvokable
{
public:
    VMPrimitive();
    VMPrimitive(MethodGenerationContext* mgenc);
    virtual ~VMPrimitive();
    virtual bool is_primitive() { return true; };
    virtual void invoke(VMFrame*);
    virtual bool IsEmpty();
    virtual void SetRoutine(Routine<Primitive>* rtn);
    virtual void MarkReferences();
private:
    bool empty;
    Routine<Primitive>* routine;
    VMSymbol* signature;
    VMClass* holder;
};

#endif