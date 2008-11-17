#include "VMFrame.h"
#include "VMMethod.h"
#include "VMObject.h"
#include "VMInteger.h"
#include "VMClass.h"
#include "VMSymbol.h"
#include "../vm/Universe.h"

VMFrame* VMFrame::EmergencyFrameFrom( VMFrame* from, int extraLength )
{
    int length = from->GetNumberOfIndexableFields() + extraLength;
    int additionalBytes = length * sizeof(VMObject*);
    VMFrame* result = new (_HEAP, additionalBytes) VMFrame(length);
    
    result->SetClass(from->GetClass());
    //copy arguments, locals and the stack
    from->CopyIndexableFieldsTo(result);

    //set Frame members
    result->SetPreviousFrame(from->GetPreviousFrame());
    result->SetMethod(from->GetMethod());
    result->SetContext(from->GetContext());
    result->stack_pointer = from->GetStackPointer();
    result->bytecode_index = from->bytecode_index;
    result->local_offset = from->local_offset;

    return result;
}


VMFrame::VMFrame(int size, int nof) : VMArray(size, nof + FRAME_NUMBER_OF_FIELDS)
{
    _HEAP->StartUninterruptableAllocation();
    this->local_offset = _UNIVERSE->NewInteger(0);//new (_HEAP) VMInteger(0);
    this->bytecode_index = _UNIVERSE->NewInteger(0);//new (_HEAP) VMInteger(0);
    this->stack_pointer = _UNIVERSE->NewInteger(0);//new (_HEAP) VMInteger(0);
    _HEAP->EndUninterruptableAllocation();
}
//
//VMFrame::~VMFrame()
//{
//}


VMFrame* VMFrame::GetPreviousFrame()
{
    return (VMFrame*) this->previous_frame;
}


void     VMFrame::SetPreviousFrame(VMObject* frm)
{
    this->previous_frame = (VMFrame*)frm;
}


void     VMFrame::ClearPreviousFrame()
{
    this->previous_frame = (VMFrame*)Globals::NilObject();
}


bool     VMFrame::HasPreviousFrame()
{
    return this->previous_frame != Globals::NilObject();
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
    return this->context !=  Globals::NilObject(); //Globals::NilObject();
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

int VMFrame::RemainingStackSize()
{
    // - 1 because the stack pointer points at the top entry,
    // so the next entry would be put at stack_pointer+1
    return this->GetNumberOfIndexableFields() - stack_pointer->GetEmbeddedInteger() - 1;
}

VMObject* VMFrame::Pop()
{
    int32_t sp = this->stack_pointer->GetEmbeddedInteger();
    this->stack_pointer->SetEmbeddedInteger(sp-1);
    return (*this)[sp];
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
        VMObject* vmo = (*this)[i];
        cout << i << ": ";
        if (vmo == NULL) cout << "NULL" << endl;
        if (vmo == Globals::NilObject()) cout << "NIL_OBJECT" << endl;
        if (vmo->GetClass() == NULL) cout << "VMObject with Class == NULL" << endl;
        if (vmo->GetClass() == Globals::NilObject()) cout << "VMObject with Class == NIL_OBJECT" << endl;
        else cout << "index: " << i << " object:" << vmo->GetClass()->GetName()->GetChars() << endl;
    }
}


void      VMFrame::ResetStackPointer()
{
    // arguments are stored in front of local variables
    VMMethod* meth = this->GetMethod();
    size_t lo = meth->GetNumberOfArguments();
    this->local_offset->SetEmbeddedInteger(lo);
  
    // Set the stack pointer to its initial value thereby clearing the stack
    size_t num_lo = meth->GetNumberOfLocals();
    this->stack_pointer->SetEmbeddedInteger(lo + num_lo - 1);
    //cout << "lo: " << lo << ", num_lo: " << num_lo << ", sp: "<<(lo+num_lo-1)<<endl;
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
    return (*this)[sp-index];
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
    return (*context)[lo+index];
}


void      VMFrame::SetLocal(int index, int contextLevel, VMObject* value)
{
    VMFrame* context = this->GetContextLevel(contextLevel);
    size_t lo = context->local_offset->GetEmbeddedInteger();
    context->SetIndexableField(lo+index, value);
}


VMInteger* VMFrame::GetStackPointer()
{
    return stack_pointer;
}


VMObject* VMFrame::GetArgument(int index, int contextLevel)
{
    // get the context
    VMFrame* context = this->GetContextLevel(contextLevel);
    return (*context)[index];
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
    return meth->GetNumberOfArguments() - index - 1;
}


void      VMFrame::CopyArgumentsFrom(VMFrame* frame)
{
    // copy arguments from frame:
    // - arguments are at the top of the stack of frame.
    // - copy them into the argument area of the current frame
    VMMethod* meth = this->GetMethod();
    int num_args = meth->GetNumberOfArguments();
    for(int i=0; i < num_args; ++i) 
    {
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
    if (gcfield) return;
     VMArray::MarkReferences();

     //previous_frame->MarkReferences();
    
     //context->MarkReferences();
     //method->MarkReferences();
     //stack_pointer->MarkReferences();
     //bytecode_index->MarkReferences();
     //local_offset->MarkReferences();
}
