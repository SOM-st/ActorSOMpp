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

    void        SetMethod(VMMethod*);
    VMMethod*   GetMethod();
    void        SetContext(VMFrame*);
    VMFrame*    GetContext();

    static VMEvaluationPrimitive* GetEvaluationPrimitive(int);
private:
    VMMethod*   blockMethod;
    VMFrame*    context;
};

#endif
