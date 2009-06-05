/*
 *  Interpreter.cpp
 *  CSOM
 *
 *  Created by Stefan Marr on 28/05/09.
 *  Copyright 2009 Vrije Universiteit Brussel. All rights reserved.
 *
 */

#include "../interpreter/Interpreter.h"
#include "../Interpreter/bytecodes.h"
#include "../vm/Universe.h"
#include "../vmobjects/VMMethod.h"
#include "../vmobjects/VMFrame.h"

#include "ActorMessaging.h"


// co-routine support
ExtendedList<pVMObject> activations;  // list of VMFrames representing co-routine activations



pVMMethod _get_method_process_incomming_msgs(SomMessage* msg) {
    pVMMethod method = _UNIVERSE->NewMethod(_UNIVERSE->SymbolForChars("process_incomming_msgs"), 3, 1);
    method->SetNumberOfLocals(0);
    method->SetMaximumNumberOfStackElements(1 + msg->GetNumberOfArguments());
    
    method->SetBytecode(0, BC_SEND);
    method->SetBytecode(1, 0);  // index for constant, are going to set constant 0 to the symbol
    method->SetBytecode(2, BC_HALT);
    
    pVMSymbol sig = _UNIVERSE->SymbolForChars(msg->GetSignature());
    method->SetIndexableField(0, sig);

    method->SetHolder(systemClass);
    
    return method;
}

pVMFrame _process_next_message() {
    SomMessage* msg = ActorMessaging::ReceiveMessage();
    
    pVMMethod method = _get_method_process_incomming_msgs(msg);
    
    pVMFrame frame = _UNIVERSE->NewFrame(pVMFrame(), method);
    frame->Push(msg->GetReceiver());
    
    for (size_t i = 0; i < msg->GetNumberOfArguments(); i++) {
        frame->Push(msg->GetArgument(i));
    }
    
    delete msg;
    return frame;
}

void Interpreter::do_YIELD(int) {
    pVMFrame current_frame = GetFrame();
    activations.PushBack(current_frame);
    
    pVMFrame new_frame;
    // receive message and construct frame/co-routine
    if (ActorMessaging::HasIncommingMessages()) {
        new_frame = _process_next_message();
    } else {
        new_frame = activations.Front();
    }
    
    SetFrame(new_frame);
}

void Interpreter::do_SEND_ASYNC(int) {
#warning not implemented
}

void Interpreter::ProcessIncommingMessages() {
    while (ActorMessaging::HasIncommingMessages() || !stop) {
        pVMFrame frame = _process_next_message();
        pVMFrame new_frame;
        // receive message and construct frame/co-routine
        if (ActorMessaging::HasIncommingMessages()) {
            new_frame = _process_next_message();
        } else {
            new_frame = activations.Front();
        }
        
        SetFrame(new_frame);
        
        Start();
    }
}
