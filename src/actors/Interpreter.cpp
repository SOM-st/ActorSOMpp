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
#warning add these structures to the GC root set
// list of VMFrames representing co-routine activations, ready for execution
ExtendedList<pVMFrame>          activations;
// list of VMFrames/co-routines, waiting for the result of a syncronous remote
// message send
map<GlobalObjectId, pVMFrame>   activationsWaitingForResult;

// list of incomming object references
// used for simple communication/synchronization
ExtendedList<pVMObject>         incommingObjReferences;
ExtendedList<pVMFrame>          waitingForObjRef;

/**
 * Are there any activation which might become ready
 * for execution in the future?
 */
bool _waitingActivationsAvailable() {
    return !activations.Empty()
        || !waitingForObjRef.Empty()
        || !activationsWaitingForResult.empty();
}


void Interpreter::AddIncommingObjRef(pVMObject obj) {
    incommingObjReferences.PushBack(obj);
}

void Interpreter::WaitForObjectReference() {
    // is almost equal to do_YIELD
    pVMFrame current_frame = GetFrame();
    waitingForObjRef.PushBack(current_frame);
    
    // force interpreter to return from loop an take another activation
    SetFrame(NULL);
}

bool _have_object_for_activation() {
    return !incommingObjReferences.Empty() && !waitingForObjRef.Empty();
}

pVMFrame _prepareWaitingActForReactivation() {
    pVMFrame frame = waitingForObjRef.FrontAndPop();
    
    frame->Push(incommingObjReferences.FrontAndPop());
    
    return frame;
}




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

void Interpreter::ProcessMessage(SomMessage* msg) {
    pVMMethod method = _get_method_process_incomming_msgs(msg);
    
    pVMFrame frame = _UNIVERSE->NewFrame(pVMFrame(), method);
    frame->Push(msg->GetReceiver());
    
    for (size_t i = 0; i < msg->GetNumberOfArguments(); i++) {
        frame->Push(msg->GetArgument(i));
    }

    activations.PushBack(frame);
}

void Interpreter::ProcessMessage(SomMessageWithResult* msg) {
    pVMMethod method = _get_method_process_incomming_msgs(msg);
    
    pVMFrame frame = _UNIVERSE->NewFrame(pVMFrame(), method);
    frame->Push(msg->GetReceiver());
    
    for (size_t i = 0; i < msg->GetNumberOfArguments(); i++) {
        frame->Push(msg->GetArgument(i));
    }
    
    activations.PushBack(frame);
}

void Interpreter::do_YIELD(int) {
    pVMFrame current_frame = GetFrame();
    activations.PushBack(current_frame);
    
    // force interpreter to return from loop an take another activation
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

void Interpreter::ProcessActivations() {
    while (!stop || _waitingActivationsAvailable()) {
        ActorMessaging::ReceiveAndProcessMessages();
        
        pVMFrame nextActivation = NULL;
        if (_have_object_for_activation()) {
            nextActivation = _prepareWaitingActForReactivation();
        } else if (!activations.Empty()) {
            nextActivation = activations.FrontAndPop();
        }
        
        if (nextActivation.IsNull()) {
            usleep(1000000);
            DebugLog("Waiting...");
        } else {
            SetFrame(nextActivation);
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