#pragma once

#ifndef VMFRAME_H_
#define VMFRAME_H_


#include "VMArray.h"
class VMMethod;
class VMObject;
class VMInteger;


class VMFrame : public VMArray {
public:
    static pVMFrame EmergencyFrameFrom(pVMFrame from, int extraLength);

    VMFrame(int size, int nof = 0);
    //virtual ~VMFrame();

    virtual inline pVMFrame   GetPreviousFrame() const;
    virtual inline void       SetPreviousFrame(pVMObject);
    virtual inline void       ClearPreviousFrame();
    virtual bool       HasPreviousFrame() const;
    virtual inline bool       IsBootstrapFrame() const;
    virtual inline pVMFrame   GetContext() const;
    virtual inline void       SetContext(pVMFrame);
    virtual bool       HasContext() const;
    virtual pVMFrame   GetContextLevel(int);
    virtual pVMFrame   GetOuterContext();
    virtual inline pVMMethod  GetMethod() const;
    virtual inline void       SetMethod(pVMMethod);
    virtual pVMObject  Pop();
    virtual void       Push(pVMObject);
    virtual void       ResetStackPointer();
    virtual int        GetBytecodeIndex() const;
    virtual void       SetBytecodeIndex(int);
    virtual pVMObject  GetStackElement(int) const;
    virtual void       SetStackElement(int, pVMObject);
    virtual pVMObject  GetLocal(int, int);
    virtual void       SetLocal(int, int, pVMObject);
    virtual pVMObject  GetArgument(int, int);
    virtual void       SetArgument(int, int, pVMObject);
    virtual void       PrintStackTrace() const;
    virtual int        ArgumentStackIndex(int index) const;
    virtual void       CopyArgumentsFrom(pVMFrame frame);
    //virtual size_t GetOffset();
    virtual void       MarkReferences();
    virtual void       PrintStack() const;
    virtual inline pVMInteger GetStackPointer() const;
    virtual int        RemainingStackSize() const;
private:
#define FRAME_NUMBER_OF_FIELDS 6
    pVMFrame   previous_frame;
    pVMFrame   context;
    pVMMethod  method;
    pVMInteger stack_pointer;
    pVMInteger bytecode_index;
    pVMInteger local_offset;
};

bool     VMFrame::IsBootstrapFrame() const {
    return !HasPreviousFrame();
}

pVMFrame VMFrame::GetContext() const {
    return this->context;
}

void     VMFrame::SetContext(pVMFrame frm) {
    this->context = frm;
}

pVMInteger VMFrame::GetStackPointer() const {
    return stack_pointer;
}

pVMMethod VMFrame::GetMethod() const {
  
    return this->method;
}

void      VMFrame::SetMethod(pVMMethod method) {
    this->method = method;
}

pVMFrame VMFrame::GetPreviousFrame() const {
    return (pVMFrame) this->previous_frame;
}


void     VMFrame::SetPreviousFrame(pVMObject frm) {
    this->previous_frame = (pVMFrame)frm;
}

void     VMFrame::ClearPreviousFrame() {
    this->previous_frame = (pVMFrame)Globals::NilObject();
}


#endif
