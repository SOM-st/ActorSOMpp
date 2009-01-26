#pragma once

#ifndef VMBLOCK_H_
#define VMBLOCK_H_

class VMMethod;
class VMFrame;
class VMEvaluationPrimitive;
#include "VMObject.h"

class VMBlock : public VMObject {
public:
    VMBlock();
    //virtual ~VMBlock();
    virtual void MarkReferences();

    inline void        SetMethod(pVMMethod);
    inline pVMMethod   GetMethod() const;
    inline void        SetContext(pVMFrame);
    inline pVMFrame    GetContext() const;

    static pVMEvaluationPrimitive GetEvaluationPrimitive(int);
private:
    pVMMethod   blockMethod;
    pVMFrame    context;
};

void VMBlock::SetMethod(pVMMethod bMethod) {
    blockMethod = bMethod;
}


pVMMethod VMBlock::GetMethod() const {
    return blockMethod;
}


void VMBlock::SetContext(pVMFrame contxt) {
    context = contxt;
}


pVMFrame VMBlock::GetContext() const {
    return context;
}

#endif
