#include "VMEvaluationPrimitive.h"
#include "VMSymbol.h"
#include "VMObject.h"
#include "VMFrame.h"
#include "VMBlock.h"
#include "VMInteger.h"
#include "../vm/Universe.h"

//needed to instanciate the Routine object for the evaluation routine
#include "../primitivesCore/Routine.h"


VMEvaluationPrimitive::VMEvaluationPrimitive(int argc) : 
                       VMPrimitive(computeSignatureString(argc)) {
    _HEAP->StartUninterruptableAllocation();
    this->SetRoutine(new Routine<VMEvaluationPrimitive>(this, 
                               &VMEvaluationPrimitive::evaluationRoutine));
    this->SetEmpty(false);
    this->numberOfArguments = _UNIVERSE->NewInteger(argc);
    _HEAP->EndUninterruptableAllocation();
}


//VMEvaluationPrimitive::~VMEvaluationPrimitive()
//{
//}
void VMEvaluationPrimitive::MarkReferences() {
    VMPrimitive::MarkReferences();
    this->numberOfArguments->MarkReferences();
}



pVMSymbol VMEvaluationPrimitive::computeSignatureString(int argc){
#define VALUE_S "value"
#define VALUE_LEN 5
#define WITH_S    "with:"
#define WITH_LEN (4+1)
#define COLON_S ":"
    
    StdString signature_string;
    
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

void VMEvaluationPrimitive::evaluationRoutine(pVMObject object, pVMFrame frame){
    pVMEvaluationPrimitive self = (pVMEvaluationPrimitive) object;

     // Get the block (the receiver) from the stack
    int num_args = self->numberOfArguments->GetEmbeddedInteger();
    pVMBlock block = (pVMBlock) frame->GetStackElement(num_args - 1);
    
    // Get the context of the block...
    pVMFrame context = block->GetContext();
    
    // Push a new frame and set its context to be the one specified in the block
    pVMFrame NewFrame = _UNIVERSE->GetInterpreter()->PushNewFrame(
                                                        block->GetMethod());
    NewFrame->CopyArgumentsFrom(frame);
    NewFrame->SetContext(context);
}
