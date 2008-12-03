#pragma once

#ifndef VMPRIMITVE_H_
#define VMPRIMITVE_H_
#include "VMObject.h"
#include "PrimitiveRoutine.h"
#include "../compiler/MethodGenerationContext.h"
#include "VMInvokable.h"
class VMSymbol;

class VMPrimitive : public VMInvokable { //public VMObject, 
public:
    VMPrimitive(VMSymbol* sig);
    //virtual ~VMPrimitive();
    virtual bool    IsPrimitive() const { return true; };
    virtual inline bool    IsEmpty() const;
    virtual inline void    SetRoutine(PrimitiveRoutine* rtn);
    virtual void    MarkReferences();
    virtual void    SetEmpty(bool value) { empty = (bool*)value; };

    //operator "()" to invoke the primitive
    virtual void    operator()(VMFrame* frm) { (*routine)(this, frm); };

    static VMPrimitive* GetEmptyPrimitive(VMSymbol* sig);
private:
    void EmptyRoutine(VMObject* self, VMFrame* frame);

    PrimitiveRoutine* routine;
    bool* empty;
    
};
bool VMPrimitive::IsEmpty() const {
    return (bool)empty;
}


void VMPrimitive::SetRoutine(PrimitiveRoutine* rtn) {
    routine = rtn;
}

#endif
