#include "VMEvaluationPrimitive.h"
#include "VMSymbol.h"
#include "VMObject.h"
#include "VMFrame.h"
#include "VMInteger.h"

VMEvaluationPrimitive::VMEvaluationPrimitive(int argc) : VMPrimitive()
{
}

VMEvaluationPrimitive::~VMEvaluationPrimitive()
{
}

void VMEvaluationPrimitive::MarkReferences()
{
    VMPrimitive::MarkReferences();
    numberOfArguments->MarkReferences();
}

VMSymbol* VMEvaluationPrimitive::computeSignatureString(int argc)
{
   /* #define VALUE_S "value"
#define VALUE_LEN 5
#define WITH_S    "with:"
#define WITH_LEN (4+1)
#define COLON_S ":"
    
    pString signature_string = String_new("");
    
    // Compute the signature string
    if(argc==1) {
        SEND(signature_string, concatChars, VALUE_S);
    } else {
        SEND(signature_string, concatChars, VALUE_S COLON_S); 
        --argc;
        while(--argc) 
            // Add extra value: selector elements if necessary
            SEND(signature_string, concatChars, WITH_S);
    }

    // Return the signature string
    return Universe_symbol_for(signature_string);*/
    return NULL;
}

void VMEvaluationPrimitive::routine(VMObject *object, VMFrame *frame)
{
    //pVMEvaluationPrimitive self = (pVMEvaluationPrimitive)object;
    //// Get the block (the receiver) from the stack
    //int num_args = SEND(self->number_of_arguments, get_embedded_integer);
    //pVMBlock block = (pVMBlock)SEND(frame, get_stack_element, num_args - 1);
    //
    //// Get the context of the block...
    //pVMFrame context = SEND(block, get_context);
    //
    //// Push a new frame and set its context to be the one specified in the block
    //pVMFrame new_frame;
    //
    //if (universe_use_threaded_interpretation) 
    //    new_frame = Interpreter_threaded_push_new_frame(SEND(block, get_method));
    //else
    //    new_frame = Interpreter_push_new_frame(SEND(block, get_method));
    //    
    //SEND(new_frame, copy_arguments_from, frame);
    //SEND(new_frame, set_context, context);    
}