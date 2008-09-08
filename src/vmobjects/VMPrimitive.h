#pragma once

#ifndef VMPRIMITVE_H_
#define VMPRIMITVE_H_
#include "VMObject.h"
#include "PrimitiveRoutine.h"
#include "../compiler/MethodGenerationContext.h"
#include "VMInvokable.h"
class VMSymbol;

class VMPrimitive : public VMInvokable //public VMObject, 
{
public:
    VMPrimitive(VMSymbol* sig);
    virtual ~VMPrimitive();
    virtual bool is_primitive() { return true; };
    virtual void invoke(VMFrame*);
    virtual bool IsEmpty();
    virtual void SetRoutine(PrimitiveRoutine* rtn);
    virtual void MarkReferences();
    virtual void SetEmpty(bool value) { *empty = value; };
    static VMPrimitive* GetEmptyPrimitive(VMSymbol* sig);

    //static void empty_routine(VMObject* self, VMFrame* frame);
    
private:
    bool* empty;
    //VMObject*   : 0; //for alignment inside the fields
    PrimitiveRoutine* routine;
    VMSymbol* signature;
    VMClass* holder;
    void empty_routine(VMObject* self, VMFrame* frame);
};

#endif