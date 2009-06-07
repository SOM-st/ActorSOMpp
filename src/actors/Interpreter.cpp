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
#include "../vmobjects/VMSymbol.h"
#include "../vmobjects/Signature.h"
#include "../misc/debug.h"

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
    SomMessage* msg = ActorMessaging::ReceiveSomMessage();
    
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

void _send_async_message(pVMObject receiver, pVMSymbol signature, size_t numOfArgs, pVMFrame arguments) {
#warning argument order of the method call could get messed up here, needs a test

    GlobalObjectId receiverId = RemoteObjectManager::GetGlobalId(receiver);
    GlobalObjectId argumentIds[numOfArgs - 1];
    
    for (size_t i = 0; i < numOfArgs - 1; i++) {
        argumentIds[i] = RemoteObjectManager::GetGlobalId(arguments->Pop());
        // have to be push on the remote frame in the correct order
        // (starting from last element)
    }
    
    SomMessage msg(receiverId,
                   signature->GetChars(), numOfArgs - 1, (GlobalObjectId*)&argumentIds);
    ActorMessaging::SendMessage(&msg, receiverId.actor_id);
    
}

void Interpreter::do_SEND_ASYNC(int bytecodeIndex) {
    DebugLog("do_SEND_ASYNC\n");
    pVMMethod method = GetMethod();
    
    pVMSymbol signature = (pVMSymbol) method->GetConstant(bytecodeIndex);
    
    size_t numOfArgs = Signature::GetNumberOfArguments(signature);
    pVMObject receiver = GetFrame()->GetStackElement(numOfArgs-1);
    
    _send_async_message(receiver, signature, numOfArgs, GetFrame());
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
