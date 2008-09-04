#pragma once

#ifndef VMPRIMITVE_H_
#define VMPRIMITVE_H_
#include "VMObject.h"
#include "Routine.h"
#include "../compiler/MethodGenerationContext.h"
#include "../primitives/Primitive.h"
#include "VMInvokable.h"
class VMSymbol;

class VMPrimitive : public VMObject, public VMInvokable, Primitive
{
public:
    VMPrimitive(VMSymbol* sig);
    virtual ~VMPrimitive();
    virtual bool is_primitive() { return true; };
    virtual void invoke(VMFrame*);
    virtual bool IsEmpty();
    virtual void SetRoutine(Routine* rtn);
    virtual void MarkReferences();

    static VMPrimitive* GetEmptyPrimitive(VMSymbol* sig);

    //static void empty_routine(VMObject* self, VMFrame* frame);
protected:
    void SetEmpty(bool value) { empty = value; };
private:
    bool empty;
    Routine* routine;
    VMSymbol* signature;
    VMClass* holder;
    static void empty_routine(VMObject* self, VMFrame* frame);
};

#endif