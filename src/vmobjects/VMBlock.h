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

    inline void        SetMethod(VMMethod*);
    inline VMMethod*   GetMethod() const;
    inline void        SetContext(VMFrame*);
    inline VMFrame*    GetContext() const;

    static VMEvaluationPrimitive* GetEvaluationPrimitive(int);
private:
    VMMethod*   blockMethod;
    VMFrame*    context;
};

void VMBlock::SetMethod(VMMethod* bMethod) {
    blockMethod = bMethod;
}


VMMethod* VMBlock::GetMethod() const {
    return blockMethod;
}


void VMBlock::SetContext(VMFrame* contxt) {
    context = contxt;
}


VMFrame* VMBlock::GetContext() const {
    return context;
}

#endif
