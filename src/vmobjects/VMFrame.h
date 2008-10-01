#pragma once

#ifndef VMFRAME_H_
#define VMFRAME_H_

#include "VMArray.h"
class VMMethod;
class VMObject;
class VMInteger;

class VMFrame : public VMArray
{
public:
    VMFrame(int size, int nof = 0);
    //virtual ~VMFrame();

    virtual VMFrame* GetPreviousFrame();
    virtual void     SetPreviousFrame(VMObject*);
    virtual void     ClearPreviousFrame();
    virtual bool     HasPreviousFrame();
    virtual bool     IsBootstrapFrame();
    virtual VMFrame* GetContext();
    virtual void     SetContext(VMFrame*);
    virtual bool     HasContext();
    virtual VMFrame* GetContextLevel(int);
    virtual VMFrame* GetOuterContext();
    virtual VMMethod* GetMethod();
    virtual void      SetMethod(VMMethod*);
    virtual VMObject* Pop();
    virtual void      Push(VMObject*);
    virtual void      ResetStackPointer();
    virtual int       GetBytecodeIndex();
    virtual void      SetBytecodeIndex(int);
    virtual VMObject* GetStackElement(int);
    virtual void      SetStackElement(int, VMObject*);
    virtual VMObject* GetLocal(int, int);
    virtual void      SetLocal(int, int, VMObject*);
    virtual VMObject* GetArgument(int, int);
    virtual void      SetArgument(int, int, VMObject*);
    virtual void      PrintStackTrace();
    virtual int       ArgumentStackIndex(int index);
    virtual void      CopyArgumentsFrom(VMFrame* frame);
    //virtual size_t GetOffset();
    virtual void MarkReferences();
    virtual void PrintStack();
    virtual VMInteger* GetStackPointer();
private:
#define FRAME_NUMBER_OF_FIELDS 6
    VMFrame*   previous_frame;
    VMFrame*   context;
    VMMethod*  method;
    VMInteger* stack_pointer;
    VMInteger* bytecode_index;
    VMInteger* local_offset;
};

#endif
