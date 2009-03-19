#include "VMFrame.h"
#include "VMMethod.h"
#include "VMObject.h"
#include "VMInteger.h"
#include "VMClass.h"
#include "VMSymbol.h"

#include "../vm/Universe.h"

//when doesNotUnderstand or UnknownGlobal is sent, additional stack slots might
//be necessary, as these cases are not taken into account when the stack
//depth is calculated. In that case this method is called.
pVMFrame VMFrame::EmergencyFrameFrom( pVMFrame from, int extraLength ) {
    int length = from->GetNumberOfIndexableFields() + extraLength;
    int additionalBytes = length * sizeof(pVMObject);
    pVMFrame result = new (_HEAP, additionalBytes) VMFrame(length);
    
    result->SetClass(from->GetClass());
    //copy arguments, locals and the stack
    from->CopyIndexableFieldsTo(result);

    //set Frame members
    result->SetPreviousFrame(from->GetPreviousFrame());
    result->SetMethod(from->GetMethod());
    result->SetContext(from->GetContext());
    result->stackPointer = from->GetStackPointer();
    result->bytecodeIndex = from->bytecodeIndex;
    result->localOffset = from->localOffset;

    return result;
}


const int VMFrame::VMFrameNumberOfFields = 6; 

VMFrame::VMFrame(int size, int nof) : VMArray(size, 
                                              nof + VMFrameNumberOfFields) {
    _HEAP->StartUninterruptableAllocation();
    this->localOffset = _UNIVERSE->NewInteger(0);//new (_HEAP) VMInteger(0);
    this->bytecodeIndex = _UNIVERSE->NewInteger(0);//new (_HEAP) VMInteger(0);
    this->stackPointer = _UNIVERSE->NewInteger(0);//new (_HEAP) VMInteger(0);
    _HEAP->EndUninterruptableAllocation();
}
//
//VMFrame::~VMFrame()
//{
//}
pVMMethod VMFrame::GetMethod() const {
  
    return this->method;
}

void      VMFrame::SetMethod(pVMMethod method) {
    this->method = method;
}

bool     VMFrame::HasPreviousFrame() const {
    return this->previousFrame != Globals::NilObject();
}




bool     VMFrame::HasContext() const {
    return this->context !=  Globals::NilObject(); //Globals::NilObject();
}


pVMFrame VMFrame::GetContextLevel(int lvl) {
    pVMFrame current = this;
    while (lvl > 0) {
        current = current->GetContext();
        --lvl;
    }
    return current;
}


pVMFrame VMFrame::GetOuterContext() {
    pVMFrame current = this;
    while (current->HasContext()) {
        current = current->GetContext();
    }
    return current;
}



int VMFrame::RemainingStackSize() const {
    // - 1 because the stack pointer points at the top entry,
    // so the next entry would be put at stackPointer+1
    return this->GetNumberOfIndexableFields() - 
           stackPointer->GetEmbeddedInteger() - 1;
}

pVMObject VMFrame::Pop() {
    int32_t sp = this->stackPointer->GetEmbeddedInteger();
    this->stackPointer->SetEmbeddedInteger(sp-1);
    return (*this)[sp];
}


void      VMFrame::Push(pVMObject obj) {
    int32_t sp = this->stackPointer->GetEmbeddedInteger() + 1;
    this->stackPointer->SetEmbeddedInteger(sp);
    (*this)[sp] = obj; //->SetIndexableField(sp, obj);
}


void VMFrame::PrintStack() const {
    cout << "SP: " << this->stackPointer->GetEmbeddedInteger() << endl;
    for (int i = 0; i < this->GetNumberOfIndexableFields()+1; ++i) {
        pVMObject vmo = (*this)[i];
        cout << i << ": ";
        if (vmo == NULL) 
            cout << "NULL" << endl;
        if (vmo == Globals::NilObject()) 
            cout << "NIL_OBJECT" << endl;
        if (vmo->GetClass() == NULL) 
            cout << "VMObject with Class == NULL" << endl;
        if (vmo->GetClass() == Globals::NilObject()) 
            cout << "VMObject with Class == NIL_OBJECT" << endl;
        else 
            cout << "index: " << i << " object:" 
                 << vmo->GetClass()->GetName()->GetChars() << endl;
    }
}


void      VMFrame::ResetStackPointer() {
    // arguments are stored in front of local variables
    pVMMethod meth = this->GetMethod();
    size_t lo = meth->GetNumberOfArguments();
    this->localOffset->SetEmbeddedInteger(lo);
  
    // Set the stack pointer to its initial value thereby clearing the stack
    size_t numLocals = meth->GetNumberOfLocals();
    this->stackPointer->SetEmbeddedInteger(lo + numLocals - 1);
    //cout << "lo: " << lo << ", num_lo: " << num_lo << ", sp: "<<(lo+num_lo-1)<<endl;
}


int       VMFrame::GetBytecodeIndex() const {
    return this->bytecodeIndex->GetEmbeddedInteger();
}


void      VMFrame::SetBytecodeIndex(int index) {
    this->bytecodeIndex->SetEmbeddedInteger(index);
}


pVMObject VMFrame::GetStackElement(int index) const {
    int sp = this->stackPointer->GetEmbeddedInteger();
    return (*this)[sp-index];
}


void      VMFrame::SetStackElement(int index, pVMObject obj) {
    int sp = this->stackPointer->GetEmbeddedInteger();
    (*this)[sp-index] = obj; //->SetIndexableField(sp-index, obj);
}


pVMObject VMFrame::GetLocal(int index, int contextLevel) {
    pVMFrame context = this->GetContextLevel(contextLevel);
    int32_t lo = context->localOffset->GetEmbeddedInteger();
    return (*context)[lo+index];
}


void      VMFrame::SetLocal(int index, int contextLevel, pVMObject value) {
    pVMFrame context = this->GetContextLevel(contextLevel);
    size_t lo = context->localOffset->GetEmbeddedInteger();
    (*context)[lo+index] = value; //->SetIndexableField(lo+index, value);
}



pVMObject VMFrame::GetArgument(int index, int contextLevel) {
    // get the context
    pVMFrame context = this->GetContextLevel(contextLevel);
    return (*context)[index];
}


void      VMFrame::SetArgument(int index, int contextLevel, pVMObject value) {
    pVMFrame context = this->GetContextLevel(contextLevel);
    (*context)[index] = value; //->SetIndexableField(index, value);
}


void      VMFrame::PrintStackTrace() const {
    //TODO
}

int       VMFrame::ArgumentStackIndex(int index) const {
    pVMMethod meth = this->GetMethod();
    return meth->GetNumberOfArguments() - index - 1;
}


void      VMFrame::CopyArgumentsFrom(pVMFrame frame) {
    // copy arguments from frame:
    // - arguments are at the top of the stack of frame.
    // - copy them into the argument area of the current frame
    pVMMethod meth = this->GetMethod();
    int num_args = meth->GetNumberOfArguments();
    for(int i=0; i < num_args; ++i) {
        pVMObject stackElem = frame->GetStackElement(num_args - 1 - i);
        (*this)[i] = stackElem;
    }
}

//
//size_t VMFrame::GetOffset()
//{
//    return VMArray::GetOffset() + sizeof(pVMFrame)*2 + sizeof(pVMMethod) + sizeof(pVMInteger)*3;
//}


void VMFrame::MarkReferences() {
    if (gcfield) return;
     VMArray::MarkReferences();
}
