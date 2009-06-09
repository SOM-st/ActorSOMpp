/*
 *  Interpreter.cpp
 *  SOM++
 *
 *  Created by Stefan Marr on 28/05/09.
 *  Copyright 2009 Vrije Universiteit Brussel. All rights reserved.
 *
 */

#include "../interpreter/Interpreter.h"
#include "../Interpreter/bytecodes.h"
#include "../vm/Universe.h"
#include "../vmobjects/VMClass.h"
#include "../vmobjects/VMMethod.h"
#include "../vmobjects/VMFrame.h"
#include "../vmobjects/VMSymbol.h"
#include "../vmobjects/Signature.h"
#include "../misc/debug.h"

#include "ActorMessaging.h"

#include <map>
#include <typeinfo>

// co-routine support
// list of VMFrames representing co-routine activations, ready for execution
ExtendedList<pVMFrame>          activations;
// list of VMFrames/co-routines, waiting for the result of a syncronous remote
// message send
map<GlobalObjectId, pVMFrame>   activationsWaitingForResult;

void Interpreter::HandleRemoteReturn(GlobalObjectId waitingActivation,
                                     pVMObject result) {
    pVMFrame activation = activationsWaitingForResult[waitingActivation];
    activation->Pop(); // think we have to pop the self here, was the receiver
    activation->Push(result);
    
    if (!result.IsImmediateInteger()) {
        DebugError("Returned value is not an int......\n");
    } else {
        int32_t const val = result.GetEmbeddedInteger();
        DebugLog("Returned Value: %d\n", val);
    }
    
    
    activationsWaitingForResult.erase(waitingActivation);
    
    activations.PushBack(activation);
}


pVMMethod _get_method_process_incomming_msgs(SomMessageWithResult* msg) {
    pVMMethod method = _UNIVERSE->NewMethod(_UNIVERSE->SymbolForChars("process_incomming_msgs"), 5, 2);
    method->SetNumberOfLocals(0);
    method->SetMaximumNumberOfStackElements(1 + msg->GetNumberOfArguments());
    
    method->SetBytecode(0, BC_SEND);
    method->SetBytecode(1, 0);  // index for constant, are going to set constant 0 to the symbol
    method->SetBytecode(2, BC_RETURN_REMOTE);
    method->SetBytecode(3, 1);
    method->SetBytecode(4, BC_HALT);
    
    pVMSymbol sig = _UNIVERSE->SymbolForChars(msg->GetSignature());
    method->SetIndexableField(0, sig);
    method->SetIndexableField(1, msg->GetResultActivation());
    
    
#warning hope this will work without holder
    //method->SetHolder(systemClass); 
    
    return method;
}

pVMMethod _get_method_process_incomming_msgs(SomMessage* msg) {
    pVMMethod method = _UNIVERSE->NewMethod(_UNIVERSE->SymbolForChars("process_incomming_msgs"), 3, 1);
    method->SetNumberOfLocals(0);
    method->SetMaximumNumberOfStackElements(1 + msg->GetNumberOfArguments());
    
    method->SetBytecode(0, BC_SEND);
    method->SetBytecode(1, 0);  // index for constant, are going to set constant 0 to the symbol
    method->SetBytecode(2, BC_HALT);
    
    pVMSymbol sig = _UNIVERSE->SymbolForChars(msg->GetSignature());
    method->SetIndexableField(0, sig);

#warning hope this will work without holder
    //method->SetHolder(systemClass); 
    
    return method;
}

pVMFrame _process_next_message() {
    SomMessage* msg = ActorMessaging::ReceiveSomMessage();

    DebugLog("Received SomMessage: %s>>%s\n", 
             msg->GetReceiver()->GetClass()->GetName()->GetChars(), 
             msg->GetSignature());
    
    pVMMethod method;
    if (msg->GetType() == SOM_MSG_WITH_RESULT) {
        method = _get_method_process_incomming_msgs((SomMessageWithResult*)msg);
    } else {
        method = _get_method_process_incomming_msgs(msg);
    }
    
    pVMFrame frame = _UNIVERSE->NewFrame(pVMFrame(), method);
    frame->Push(msg->GetReceiver());
    
    for (size_t i = 0; i < msg->GetNumberOfArguments(); i++) {
        frame->Push(msg->GetArgument(i));
    }
    
    delete msg;
    return frame;
}


/*
void Interpreter::setNextActivation() {
    pVMFrame new_frame = _wait_for_next_activation();
    SetFrame(new_frame);
}*/

void Interpreter::do_YIELD(int) {
    pVMFrame current_frame = GetFrame();
    activations.PushBack(current_frame);

    //setNextActivation(); refactored to ProcessIncommingMessages
    SetFrame(NULL);
}

void _send_async_message(pVMObject receiver, pVMSymbol signature, 
                         size_t numOfArgs, pVMFrame arguments) {
#warning argument order of the method call could get messed up here, needs a test

    GlobalObjectId receiverId = RemoteObjectManager::GetGlobalId(receiver);
    GlobalObjectId argumentIds[numOfArgs - 1];
    
    for (int i = numOfArgs - 2; i >= 0 ; i--) {
        argumentIds[i] = RemoteObjectManager::GetGlobalId(arguments->Pop());
    }
    
    SomMessage msg(receiverId, signature->GetChars(), numOfArgs - 1,
                   (GlobalObjectId*)&argumentIds);
    ActorMessaging::SendMessage(&msg, receiverId.actor_id);
    
}

void Interpreter::sendSyncMessage(pVMObject receiver, pVMSymbol signature, 
                                  size_t numOfArgs, pVMFrame currentFrame) {

    GlobalObjectId activation = RemoteObjectManager::GetGlobalId(currentFrame);
    activationsWaitingForResult[activation] = currentFrame;
    
    GlobalObjectId receiverId = RemoteObjectManager::GetGlobalId(receiver);
    GlobalObjectId argumentIds[numOfArgs - 1];

    for (int i = numOfArgs - 2; i >= 0 ; i--) {
        argumentIds[i] = RemoteObjectManager::GetGlobalId(currentFrame->Pop());
    }
    
    SomMessageWithResult msg(receiverId, signature->GetChars(), numOfArgs - 1,
                             (GlobalObjectId*)&argumentIds, activation);
    ActorMessaging::SendMessage(&msg, receiverId.actor_id);
    
    //setNextActivation(); refactored to ProcessIncommingMessages
    SetFrame(NULL);
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
        pVMFrame new_frame = NULL;
        
        // receive message and construct frame/co-routine
        if (ActorMessaging::HasIncommingMessages()) {
            new_frame = _process_next_message();
        } else if (!activations.Empty()) {
            new_frame = activations.Front();
            activations.PopFront();
        }
        
        if (new_frame.IsNull()) {
            usleep(1000000);
            DebugLog("Waiting...");
        } else {
            SetFrame(new_frame);
            Start();
        }        
    }
    DebugLog("Completed ProcessIncommingMessages\n");
}

void Interpreter::do_RETURN_REMOTE(int bytecodeIndex) {
    DebugLog("do_RETURN_REMOTE\n");
    pVMMethod method = GetMethod();
    
    pVMObject resultActivation = (pVMRemoteObject)method->GetConstant(bytecodeIndex);
    pVMObject result = GetFrame()->Pop();

    GlobalObjectId resultActId = RemoteObjectManager::GetGlobalId(resultActivation);
    ResultObjRefMessage msg(RemoteObjectManager::GetGlobalId(result), 
                            resultActId);
    ActorMessaging::SendMessage(&msg, resultActId.actor_id);
}