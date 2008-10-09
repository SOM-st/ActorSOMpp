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
    //virtual ~VMPrimitive();
    virtual bool    IsPrimitive() { return true; };
    virtual void    Invoke(VMFrame*);
    virtual bool    IsEmpty();
    virtual void    SetRoutine(PrimitiveRoutine* rtn);
    virtual void    MarkReferences();
    virtual void    SetEmpty(bool value) { *empty = value; };

    static VMPrimitive* GetEmptyPrimitive(VMSymbol* sig);
private:
    void EmptyRoutine(VMObject* self, VMFrame* frame);

    PrimitiveRoutine* routine;
    bool* empty;
    
};

#endif
