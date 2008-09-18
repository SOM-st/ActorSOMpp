#include "VMFrame.h"
#include "VMMethod.h"
#include "VMObject.h"
#include "VMInteger.h"
#include "VMClass.h"
#include "VMSymbol.h"
#include "../vm/Universe.h"

VMFrame::VMFrame(int size, int nof) : VMArray(size, FRAME_NUMBER_OF_FIELDS)
{
   // this->SetNumberOfFields(this->GetNumberOfFields() + 6);
    //this->objectSize = sizeof(VMFrame) + size*sizeof(VMObject*);
    this->local_offset = new (_HEAP) VMInteger(0);
    this->bytecode_index = new (_HEAP) VMInteger(0);
    this->stack_pointer = new (_HEAP) VMInteger(0);
}

VMFrame::~VMFrame()
{
}

VMFrame* VMFrame::GetPreviousFrame()
{
    return (VMFrame*) this->previous_frame;
}

void     VMFrame::SetPreviousFrame(VMObject* frm)
{
    this->fields[3] = frm; //HACK: 4 == previous_frame
    //this->previous_frame = (VMFrame*)frm;
}

void     VMFrame::ClearPreviousFrame()
{
    this->fields[3] = nil_object;
    //this->previous_frame = (VMFrame*)nil_object;
}

bool     VMFrame::HasPreviousFrame()
{
    return this->previous_frame != nil_object;
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
    return this->context !=  nil_object; //nil_object;
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
    this->stack_pointer->SetEmbeddedInteger(sp-1);
    return this->GetIndexableField(sp);
}

void      VMFrame::Push(VMObject* obj)
{
    int32_t sp = this->stack_pointer->GetEmbeddedInteger() + 1;
    this->stack_pointer->SetEmbeddedInteger(sp);
    this->SetIndexableField(sp, obj);
}

void VMFrame::PrintStack()
{
    cout << "SP: " << this->stack_pointer->GetEmbeddedInteger() << endl;
    for (int i = 0; i < this->GetNumberOfIndexableFields()+1; ++i)
    {
        VMObject* vmo = this->GetIndexableField(i);
        if (vmo == NULL) cout << "NULL" << endl;
        else cout << "index: " << i << " object:" << vmo->GetClass()->get_name()->GetChars() << endl;
    }
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
    cout << "lo: " << lo << ", num_lo: " << num_lo << ", sp: "<<(lo+num_lo-1)<<endl;
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
    this->SetIndexableField(sp-index, obj);
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
    context->SetIndexableField(lo+index, value);
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
    context->SetIndexableField(index, value);
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
        this->SetIndexableField(i, stackElem);
    }
}
//
//size_t VMFrame::GetOffset()
//{
//    return VMArray::GetOffset() + sizeof(VMFrame*)*2 + sizeof(VMMethod*) + sizeof(VMInteger*)*3;
//}

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
