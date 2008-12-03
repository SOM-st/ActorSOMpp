#pragma once

#ifndef VMFRAME_H_
#define VMFRAME_H_


#include "VMArray.h"
class VMMethod;
class VMObject;
class VMInteger;


class VMFrame : public VMArray {
public:
    static VMFrame* EmergencyFrameFrom(VMFrame* from, int extraLength);

    VMFrame(int size, int nof = 0);
    //virtual ~VMFrame();

    virtual inline VMFrame*   GetPreviousFrame() const;
    virtual inline void       SetPreviousFrame(VMObject*);
    virtual inline void       ClearPreviousFrame();
    virtual bool       HasPreviousFrame() const;
    virtual inline bool       IsBootstrapFrame() const;
    virtual inline VMFrame*   GetContext() const;
    virtual inline void       SetContext(VMFrame*);
    virtual bool       HasContext() const;
    virtual VMFrame*   GetContextLevel(int);
    virtual VMFrame*   GetOuterContext();
    virtual inline VMMethod*  GetMethod() const;
    virtual inline void       SetMethod(VMMethod*);
    virtual VMObject*  Pop();
    virtual void       Push(VMObject*);
    virtual void       ResetStackPointer();
    virtual int        GetBytecodeIndex() const;
    virtual void       SetBytecodeIndex(int);
    virtual VMObject*  GetStackElement(int) const;
    virtual void       SetStackElement(int, VMObject*);
    virtual VMObject*  GetLocal(int, int);
    virtual void       SetLocal(int, int, VMObject*);
    virtual VMObject*  GetArgument(int, int);
    virtual void       SetArgument(int, int, VMObject*);
    virtual void       PrintStackTrace() const;
    virtual int        ArgumentStackIndex(int index) const;
    virtual void       CopyArgumentsFrom(VMFrame* frame);
    //virtual size_t GetOffset();
    virtual void       MarkReferences();
    virtual void       PrintStack() const;
    virtual inline VMInteger* GetStackPointer() const;
    virtual int        RemainingStackSize() const;
private:
#define FRAME_NUMBER_OF_FIELDS 6
    VMFrame*   previous_frame;
    VMFrame*   context;
    VMMethod*  method;
    VMInteger* stack_pointer;
    VMInteger* bytecode_index;
    VMInteger* local_offset;
};

bool     VMFrame::IsBootstrapFrame() const {
    return !HasPreviousFrame();
}

VMFrame* VMFrame::GetContext() const {
    return this->context;
}

void     VMFrame::SetContext(VMFrame* frm) {
    this->context = frm;
}

VMInteger* VMFrame::GetStackPointer() const {
    return stack_pointer;
}

VMMethod* VMFrame::GetMethod() const {
  
    return this->method;
}

void      VMFrame::SetMethod(VMMethod* method) {
    this->method = method;
}

VMFrame* VMFrame::GetPreviousFrame() const {
    return (VMFrame*) this->previous_frame;
}


void     VMFrame::SetPreviousFrame(VMObject* frm) {
    this->previous_frame = (VMFrame*)frm;
}

void     VMFrame::ClearPreviousFrame() {
    this->previous_frame = (VMFrame*)Globals::NilObject();
}


#endif
