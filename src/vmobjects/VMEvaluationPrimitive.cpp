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
    
    StdString signatureString;
    
    // Compute the signature string
    if(argc==1) {
        signatureString += VALUE_S;
    } else {
        signatureString += VALUE_S ;
        signatureString += COLON_S; 
        --argc;
        while(--argc) 
            // Add extra value: selector elements if necessary
            signatureString + WITH_S;
    }

    // Return the signature string
    return _UNIVERSE->SymbolFor(signatureString);
}

void VMEvaluationPrimitive::evaluationRoutine(pVMObject object, pVMFrame frame){
    pVMEvaluationPrimitive self = (pVMEvaluationPrimitive) object;

     // Get the block (the receiver) from the stack
    int numArgs = self->numberOfArguments->GetEmbeddedInteger();
    pVMBlock block = (pVMBlock) frame->GetStackElement(numArgs - 1);
    
    // Get the context of the block...
    pVMFrame context = block->GetContext();
    
    // Push a new frame and set its context to be the one specified in the block
    pVMFrame NewFrame = _UNIVERSE->GetInterpreter()->PushNewFrame(
                                                        block->GetMethod());
    NewFrame->CopyArgumentsFrom(frame);
    NewFrame->SetContext(context);
}
