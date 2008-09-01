#pragma once

#ifndef VMEVALUATIONPRIMITIVE_H_
#define VMEVALUATIONPRIMITIVE_H_

#include "VMPrimitive.h"
class VMInteger;
class VMObject;
class VMFrame;


class VMEvaluationPrimitive : public VMPrimitive
{
public:
    VMEvaluationPrimitive(int argc);
    virtual ~VMEvaluationPrimitive();
    virtual void MarkReferences();
private:
    VMSymbol* computeSignatureString(int argc);
    void routine(VMObject* object, VMFrame* frame);
    VMInteger* numberOfArguments;
};

#endif