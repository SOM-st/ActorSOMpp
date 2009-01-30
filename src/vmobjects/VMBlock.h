#pragma once

#ifndef VMBLOCK_H_
#define VMBLOCK_H_

#include "VMObject.h"

class VMMethod;
class VMFrame;
class VMEvaluationPrimitive;

class VMBlock : public VMObject {
public:
    VMBlock();
    //virtual ~VMBlock();
    virtual void MarkReferences();

    void        SetMethod(pVMMethod);
    pVMMethod   GetMethod() const;
    inline void        SetContext(pVMFrame);
    inline pVMFrame    GetContext() const;

    static pVMEvaluationPrimitive GetEvaluationPrimitive(int);
private:
    pVMObject   blockMethod;
    pVMFrame    context;

    static const int VMBlockNumberOfFields;
};




void VMBlock::SetContext(pVMFrame contxt) {
    context = contxt;
}


pVMFrame VMBlock::GetContext() const {
    return context;
}

#endif
