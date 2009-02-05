#pragma once

#ifndef VMPRIMITVE_H_
#define VMPRIMITVE_H_

#include "VMObject.h"
#include "PrimitiveRoutine.h"
#include "VMInvokable.h"

//#include "../compiler/MethodGenerationContext.h"

class VMSymbol;

class VMPrimitive : public VMInvokable {
public:
    static pVMPrimitive GetEmptyPrimitive(pVMSymbol sig);

    VMPrimitive(pVMSymbol sig);
    //virtual ~VMPrimitive();
    
    virtual inline bool    IsEmpty() const;
    virtual inline void    SetRoutine(PrimitiveRoutine* rtn);
    virtual void    MarkReferences();
    virtual void    SetEmpty(bool value) { empty = (bool*)value; };

    //-----------VMInvokable-------//
    //operator "()" to invoke the primitive
    virtual void    operator()(pVMFrame frm) { (*routine)(this, frm); };

    virtual bool      IsPrimitive() const { return true; };
    
private:
    void EmptyRoutine(pVMObject self, pVMFrame frame);

    PrimitiveRoutine* routine;
    bool* empty;

    static const int VMPrimitiveNumberOfFields;
    
};

bool VMPrimitive::IsEmpty() const {
    return (bool)empty;
}


void VMPrimitive::SetRoutine(PrimitiveRoutine* rtn) {
    routine = rtn;
}

#endif
