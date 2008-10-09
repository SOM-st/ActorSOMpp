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
    //virtual ~VMEvaluationPrimitive();
private:
    static VMSymbol* computeSignatureString(int argc);
    void evaluationRoutine(VMObject* object, VMFrame* frame);
    VMInteger* numberOfArguments;
};

#endif
