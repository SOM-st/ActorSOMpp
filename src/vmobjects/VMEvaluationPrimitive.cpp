#include "VMEvaluationPrimitive.h"
#include "VMSymbol.h"
#include "VMObject.h"
#include "VMFrame.h"
#include "VMBlock.h"
#include "VMInteger.h"
#include "../vm/Universe.h"

//needed to instanciate the Routine object for the evaluation routine
#include "../primitives/Routine.h"

VMEvaluationPrimitive::VMEvaluationPrimitive(int argc) : VMPrimitive(computeSignatureString(argc))
{
    _UNIVERSE->GetHeap()->StartUninterruptableAllocation();
    this->SetRoutine(new (_HEAP) Routine<VMEvaluationPrimitive>(this, &VMEvaluationPrimitive::evaluationRoutine));
    this->SetEmpty(false);
    this->numberOfArguments = _UNIVERSE->NewInteger(argc);
    _UNIVERSE->GetHeap()->EndUninterruptableAllocation();
}

//VMEvaluationPrimitive::~VMEvaluationPrimitive()
//{
//}
void VMEvaluationPrimitive::MarkReferences()
{
    if (gcfield) return;
    VMPrimitive::MarkReferences();
    this->numberOfArguments->MarkReferences();
}



VMSymbol* VMEvaluationPrimitive::computeSignatureString(int argc)
{
#define VALUE_S "value"
#define VALUE_LEN 5
#define WITH_S    "with:"
#define WITH_LEN (4+1)
#define COLON_S ":"
    
    pString signature_string;
    
    // Compute the signature string
    if(argc==1) {
        signature_string += VALUE_S;
    } else {
        signature_string += VALUE_S ;
        signature_string += COLON_S; 
        --argc;
        while(--argc) 
            // Add extra value: selector elements if necessary
            signature_string + WITH_S;
    }

    // Return the signature string
    return _UNIVERSE->SymbolFor(signature_string);
}

void VMEvaluationPrimitive::evaluationRoutine(VMObject *object, VMFrame *frame)
{
    VMEvaluationPrimitive* self = (VMEvaluationPrimitive*) object;

     // Get the block (the receiver) from the stack
    int num_args = self->numberOfArguments->GetEmbeddedInteger();
    VMBlock* block = (VMBlock*) frame->GetStackElement(num_args - 1);
    
    // Get the context of the block...
    VMFrame* context = block->GetContext();
    
    // Push a new frame and set its context to be the one specified in the block
    VMFrame* NewFrame = _UNIVERSE->GetInterpreter()->PushNewFrame(block->GetMethod());
    NewFrame->CopyArgumentsFrom(frame);
    NewFrame->SetContext(context);
}
