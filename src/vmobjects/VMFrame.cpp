#include "VMFrame.h"
#include "VMMethod.h"
#include "VMObject.h"
#include "VMInteger.h"
#include "../vm/Universe.h"

VMFrame::VMFrame(int size) : VMArray(size)
{
    this->local_offset = new (_HEAP) VMInteger(0);
    this->bytecode_index = new (_HEAP) VMInteger(0);
    this->stack_pointer = new (_HEAP) VMInteger(0);
}

VMFrame::~VMFrame()
{
}

VMFrame* VMFrame::GetPreviousFrame()
{
    return this->previous_frame;
}

void     VMFrame::SetPreviousFrame(VMFrame* frm)
{
    this->previous_frame = frm;
}

void     VMFrame::ClearPreviousFrame()
{
    this->previous_frame = NULL;//= NilObject;
}

bool     VMFrame::HasPreviousFrame()
{
    return this->previous_frame != NULL;
}

bool     VMFrame::IsBootstrapFrame()
{
    return !HasPreviousFrame();
}

VMFrame* VMFrame::GetContext()
{
    return this->context;
}

void     VMFrame::SetContext(VMFrame* frm)
{
    this->context = frm;
}

bool     VMFrame::HasContext()
{
    return this->context !=  NULL; //nil_object;
}

VMFrame* VMFrame::GetContextLevel(int lvl)
{
    VMFrame* current = this;
    while (lvl > 0)
    {
        current = current->GetContext();
        --lvl;
    }
    return current;
}

VMFrame* VMFrame::GetOuterContext()
{
    VMFrame* current = this;
    while (current->HasContext())
    {
        current = current->GetContext();
    }
    return current;
}

VMMethod* VMFrame::GetMethod()
{
    return this->method;
}

void      VMFrame::SetMethod(VMMethod* method)
{
    this->method = method;
}

VMObject* VMFrame::Pop()
{
    int32_t sp = this->stack_pointer->GetEmbeddedInteger();
    this->stack_pointer->SetEmbeddedInteger(sp - 1);
    return this->GetIndexableField(sp);
}

void      VMFrame::Push(VMObject* obj)
{
    int32_t sp = this->stack_pointer->GetEmbeddedInteger() + 1;
    this->stack_pointer->SetEmbeddedInteger(sp);
    this->AddItem(sp, obj);
}

void      VMFrame::ResetStackPointer()
{
    // arguments are stored in front of local variables
    VMMethod* meth = this->GetMethod();
    size_t lo = meth->get_number_of_arguments();
    this->local_offset->SetEmbeddedInteger(lo);
  
    // Set the stack pointer to its initial value thereby clearing the stack
    size_t num_lo = meth->get_number_of_locals();
    this->stack_pointer->SetEmbeddedInteger(lo + num_lo - 1);
}

int       VMFrame::GetBytecodeIndex()
{
    return this->bytecode_index->GetEmbeddedInteger();
}

void      VMFrame::SetBytecodeIndex(int index)
{
    this->bytecode_index->SetEmbeddedInteger(index);
}

VMObject* VMFrame::GetStackElement(int index)
{
    int sp = this->stack_pointer->GetEmbeddedInteger();
    return this->GetIndexableField(sp-index);
}

void      VMFrame::SetStackElement(int index, VMObject* obj)
{
    int sp = this->stack_pointer->GetEmbeddedInteger();
    this->AddItem(sp-index, obj);
}

VMObject* VMFrame::GetLocal(int index, int contextLevel)
{
    VMFrame* context = this->GetContextLevel(contextLevel);
    int32_t lo = context->local_offset->GetEmbeddedInteger();
    return context->GetIndexableField(lo + index);
}

void      VMFrame::SetLocal(int index, int contextLevel, VMObject* value)
{
    VMFrame* context = this->GetContextLevel(contextLevel);
    size_t lo = context->local_offset->GetEmbeddedInteger();
    context->AddItem(lo+index, value);
}

VMObject* VMFrame::GetArgument(int index, int contextLevel)
{
    // get the context
    VMFrame* context = this->GetContextLevel(contextLevel);
    return context->GetIndexableField(index);
}

void      VMFrame::SetArgument(int index, int contextLevel, VMObject* value)
{
    VMFrame* context = this->GetContextLevel(contextLevel);
    context->AddItem(index, value);
}

void      VMFrame::PrintStackTrace()
{
}

int       VMFrame::ArgumentStackIndex(int index)
{
    VMMethod* meth = this->GetMethod();
    return meth->get_number_of_arguments() - index - 1;
}

void      VMFrame::CopyArgumentsFrom(VMFrame* frame)
{
    // copy arguments from frame:
    // - arguments are at the top of the stack of frame.
    // - copy them into the argument area of the current frame
    VMMethod* meth = this->GetMethod();
    int num_args = meth->get_number_of_arguments();
    for(int i=0; i < num_args; ++i) {
        VMObject* stackElem = frame->GetStackElement(num_args - 1 - i);
        this->AddItem(i, stackElem);
    }
}

size_t VMFrame::GetOffset()
{
    return VMArray::GetOffset() + sizeof(VMFrame*)*2 + sizeof(VMMethod*) + sizeof(VMInteger*)*3;
}

void VMFrame::MarkReferences()
{
     VMArray::MarkReferences();

     previous_frame->MarkReferences();
    
     context->MarkReferences();
     method->MarkReferences();
     stack_pointer->MarkReferences();
     bytecode_index->MarkReferences();
     local_offset->MarkReferences();
}