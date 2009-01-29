#pragma once

#ifndef VMPRIMITVE_H_
#define VMPRIMITVE_H_
#include "VMObject.h"
#include "PrimitiveRoutine.h"
#include "../compiler/MethodGenerationContext.h"
#include "VMInvokable.h"
class VMSymbol;

class VMPrimitive : public VMInvokable, public VMObject {
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
    virtual pVMSymbol GetSignature() const;
	virtual void      SetSignature(pVMSymbol sig);
	virtual pVMClass  GetHolder() const;
	virtual void      SetHolder(pVMClass hld);
    virtual bool      IsPrimitive() const { return true; };
    
private:
    void EmptyRoutine(pVMObject self, pVMFrame frame);

    pVMSymbol signature;
    pVMClass holder;

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
