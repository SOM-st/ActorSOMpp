/*
 *  messages.h
 *  SOM
 *
 *  Created by Stefan Marr on 05/06/09.
 *  Copyright 2009 Vrije Universiteit Brussel. All rights reserved.
 *
 */

#ifndef _MESSAGES_H_
#define _MESSAGES_H_

#include <string.h>
#include <stdlib.h>

#include "../vmobjects/VMObject.h"
#include "../vmobjects/VMSymbol.h"
#include "../vmobjects/ObjectFormats.h"
#include "../memory/ObjectTable.h"

#include "GlobalObjectId.h"
#include "RemoteObjectManager.h"

// Defined as Bit mask and with or operator to enable filtering features in receive
typedef enum Messages {
    ABSTRACT_MSG       = 0x00,
    
    EXIT_MSG           = 0x01,   // ask an actor to stop after all incomming messages have been processed
    
    OBJ_REF_MSG        = 0x02,   // message contains a reference to an object
    OBJ_REF_RESULT_MSG = 0x04,   // message contains a reference and a pointer to the activation to be continued
    
    SOM_MSG            = 0x08,   // represents a message sent on language level,
                                 // which has to be send async to an actor
    
    SOM_MSG_WITH_RESULT= 0x10,   // message send async to an actor and expects result
    
    NEW_REF_MSG        = 0x1000, // message to notify an actor about a new reference to one of its objects  (id value choosen with some space, since this is a infrastructure msg)
    UNLINK_REF_MSG     = 0x2000, // message to notify an actor, that an object is not referenced by this other actor anymore
    
    ANY_MESSAGE        = -1      // should provide a all 1 bit pattern and satisfy any message type on a &
} Messages;

inline Messages operator|(Messages a, Messages b) {
    return Messages(int(a) | int(b));
}



class Message {
public:
    Message() : msgType(ABSTRACT_MSG) {}
    Message(Messages msgType) : msgType(msgType) {}
  
    virtual size_t GetSize() { return sizeof(int32_t); }
    
    virtual void* Serialize(void* buffer) {
        *(int32_t*)buffer = GetType();
        return (void*)((intptr_t)buffer + sizeof(int32_t));
    }
    
    virtual void Process() = 0;
    virtual void TrackObjectSends(actor_id_t receiver) = 0;

    Messages GetType() { return msgType; }
    
    static Message* Deserialize(void* buffer);
    
private:
    Messages msgType;
};



class EmptyMessage : public Message {
public:
    EmptyMessage(Messages msgType) : Message(msgType) {}
    EmptyMessage() : Message(ABSTRACT_MSG) {}
};



class ExitMsg : public EmptyMessage {
public:
    ExitMsg() : EmptyMessage(EXIT_MSG) {};
    virtual void Process();
    virtual void TrackObjectSends(actor_id_t receiver) {}
};


class NewRefNotificationMsg : public Message {
public:
    NewRefNotificationMsg(GlobalObjectId referencedObj, actor_id_t actorReceivingRef)
        : Message(NEW_REF_MSG), referencedObj(referencedObj), actor(actorReceivingRef) {}
    NewRefNotificationMsg(Messages msgType, GlobalObjectId referencedObj, actor_id_t actorReceivingRef)
        : Message(msgType), referencedObj(referencedObj), actor(actorReceivingRef) {}
    NewRefNotificationMsg(void* buffer) : Message(NEW_REF_MSG) { Deserialize(buffer); }
    NewRefNotificationMsg(Messages msgType, void* buffer) : Message(msgType) { Deserialize(buffer); }
        
    virtual void* Serialize(void* buffer) {
        buffer = Message::Serialize(buffer);
        buffer = referencedObj.SerializeDirect(buffer);
        *(actor_id_t*)buffer = actor;
        
        return (void*)((intptr_t)buffer + sizeof(actor_id_t));
    }
    
    virtual void Process();
    
    // is no-op, since this is of course issued by exactly this virtual functions
    virtual void TrackObjectSends(actor_id_t receiver) {}
    
    virtual size_t GetSize() {
        return Message::GetSize()
        + referencedObj.GetDirectSerializedSize()
        + sizeof(actor_id_t);
    }
    
protected:
    
    virtual void* Deserialize(void* buffer) {
        buffer = GlobalObjectId::Deserialize(buffer, referencedObj);
        
        actor = *(actor_id_t*)buffer;
        
        return (void*)((intptr_t)buffer + sizeof(actor_id_t));
    }

protected:
    GlobalObjectId referencedObj;
    actor_id_t actor;
};


class UnlinkRefMsg : public NewRefNotificationMsg {
public:
    UnlinkRefMsg(GlobalObjectId referencedObj, actor_id_t actorReceivingRef)
    : NewRefNotificationMsg(UNLINK_REF_MSG, referencedObj, actorReceivingRef) {}
    UnlinkRefMsg(void* buffer) : NewRefNotificationMsg(UNLINK_REF_MSG, buffer) {}
    
    virtual void Process();
};




class ObjRefMessage : public Message {
public:
    ObjRefMessage(pVMObject obj) : Message(OBJ_REF_MSG), object(obj) {}
    ObjRefMessage(Messages msgType, pVMObject obj) : object(obj), Message(msgType) {}
    ObjRefMessage(void* buffer) : Message(OBJ_REF_MSG) { Deserialize(buffer); }
    ObjRefMessage(Messages msgType) : Message(msgType) {}
    
    virtual void* Serialize(void* buffer) {
        buffer = Message::Serialize(buffer);
        buffer = object->Serialize(buffer);
        return buffer;
    }
    
    pVMObject GetObject() {
        return object;
    }
    
    virtual void Process();
    
    virtual void TrackObjectSends(actor_id_t receiver) {
        RemoteObjectManager::TrackObjectSend(object, receiver);
    }

    virtual size_t GetSize() {
        return Message::GetSize() + object->GetSerializedSize();
    }
    
protected:
    
    virtual void* Deserialize(void* buffer) {
        object = _UNIVERSE->NewObjectFromBuffer(buffer);
        return buffer;
    }
    
private:
    pVMObject object;
};



class ResultObjRefMessage : public ObjRefMessage {
public:
    ResultObjRefMessage(pVMObject result, GlobalObjectId activation)
    : ObjRefMessage(OBJ_REF_RESULT_MSG, result), resultActivation(activation) {}
    
    ResultObjRefMessage(void* buffer) : ObjRefMessage(OBJ_REF_MSG) { 
        Deserialize(buffer); 
    }
    
    virtual void* Serialize(void* buffer) {
        buffer = ObjRefMessage::Serialize(buffer);
        buffer = resultActivation.SerializeDirect(buffer);
        return buffer;
    }
    
    virtual void Process();
    
    virtual void TrackObjectSends(actor_id_t receiver) {
        ObjRefMessage::TrackObjectSends(receiver);
        RemoteObjectManager::TrackObjectSend(resultActivation, receiver);
    }    
    
    virtual size_t GetSize() {
        return ObjRefMessage::GetSize() + resultActivation.GetDirectSerializedSize();
    }

protected:
    
    virtual void* Deserialize(void* buffer) {
        buffer = ObjRefMessage::Deserialize(buffer);
        buffer = GlobalObjectId::Deserialize(buffer, resultActivation);
        return buffer;
    }
    
private:
    GlobalObjectId resultActivation;    
};



class SomMessage : public Message {
public:
    SomMessage(Messages msgType, pVMObject receiver, pVMSymbol signature,
               size_t num_args, pVMObject* arguments)
    : Message(msgType), receiver(receiver), signature(signature), 
      number_of_arguments(num_args), arguments(arguments) {
    }  
    
    SomMessage(pVMObject receiver, pVMSymbol signature, size_t num_args, 
               pVMObject* arguments)
    : Message(SOM_MSG), receiver(receiver), signature(signature), 
      number_of_arguments(num_args), arguments(arguments) {
    }
  
    SomMessage(void* buffer) : Message(SOM_MSG) {
        Deserialize(buffer);
    }
    
    SomMessage(Messages msgType) : Message(msgType) {}

    #warning missing deconstructor and memory leaks arguments array
    
    pVMSymbol GetSignature() { return signature; }
    size_t GetNumberOfArguments() { return number_of_arguments; }
    pVMObject GetReceiver() { 
        return receiver;
    }
  
    pVMObject GetArgument(size_t i) {
        return arguments[i];
    }
  
    virtual size_t GetSize() {
        // get all sizes of the arguments
        size_t argLen = 0;
        for (size_t i = 0; i < number_of_arguments; i++) {
            argLen += arguments[i]->GetSerializedSize();
        }
        
        
        return Message::GetSize()
              + receiver->GetSerializedSize()
              + signature->GetSerializedSize()
              + sizeof(size_t)
              + argLen;
    }
  
    virtual void* Serialize(void* buffer) {
        buffer = Message::Serialize(buffer);
        buffer = receiver->Serialize(buffer);
        buffer = signature->Serialize(buffer);

        size_t* num_args = (size_t*)buffer;
        *num_args = this->number_of_arguments;
        buffer = (void*)((intptr_t)num_args + sizeof(size_t));
            
        for (size_t i = 0; i < number_of_arguments; i++) {
            buffer = arguments[i]->Serialize(buffer);
        }

        return buffer;
    }
    
    virtual void TrackObjectSends(actor_id_t receivingActor) {

        RemoteObjectManager::TrackObjectSend(receiver, receivingActor);   // is actually not necessary, but this an asumption I am not sure I should rely on here, who know what will change...
        RemoteObjectManager::TrackObjectSend(signature, receivingActor);  // is actually not necessary, but this an asumption I am not sure I should rely on here, who know what will change...

        for (size_t i = 0; i < number_of_arguments; i++) {
            RemoteObjectManager::TrackObjectSend(arguments[i], receivingActor);
        }
    }    

    virtual void Process();
  
protected:
    
    virtual void* Deserialize(void* buffer) {
        receiver = _UNIVERSE->NewObjectFromBuffer(buffer);
        signature = _UNIVERSE->NewObjectFromBuffer(buffer);
        
        size_t* num_args = (size_t*)buffer;
        number_of_arguments = *num_args;
        buffer = (void*)((intptr_t)buffer + sizeof(number_of_arguments));
        
        arguments = (pVMObject*)malloc(sizeof(pVMObject) * number_of_arguments);
        
        for (size_t i = 0; i < number_of_arguments; i++) {
            arguments[i] = _UNIVERSE->NewObjectFromBuffer(buffer);
        }
                
        return buffer;
    }
    

private:
    pVMObject receiver;
    pVMSymbol signature;
    size_t number_of_arguments;
    pVMObject* arguments;
};

class SomMessageWithResult : public SomMessage {
public:
    SomMessageWithResult(pVMObject receiver, pVMSymbol signature, size_t num_args, 
               pVMObject* arguments, pVMFrame resultActivation)
    : SomMessage(SOM_MSG_WITH_RESULT, receiver, signature, num_args, arguments),
      resultActivation(resultActivation) {}
    
    SomMessageWithResult(void* buffer) : SomMessage(SOM_MSG_WITH_RESULT) {
        Deserialize(buffer);
    }
    
#warning missing deconstructor and memory leaks for arguments array    
    
    virtual size_t GetSize() {
        return SomMessage::GetSize()
              + resultActivation->GetSerializedSize();
    }
    
    virtual void* Serialize(void* buffer) {
        buffer = SomMessage::Serialize(buffer);
        buffer = resultActivation->Serialize(buffer);
        return buffer;
    }
    
    virtual void Process();
    
    virtual void TrackObjectSends(actor_id_t receivingActor) {
        SomMessage::TrackObjectSends(receivingActor);
        RemoteObjectManager::TrackObjectSend(resultActivation, receivingActor);   // is actually not necessary, but this an asumption I am not sure I should rely on here, who know what will change...
    }   
    
    pVMObject GetResultActivation() {
        return resultActivation;
    }
    
protected:
    
    virtual void* Deserialize(void* buffer) {
        buffer = SomMessage::Deserialize(buffer);
        resultActivation = _UNIVERSE->NewObjectFromBuffer(buffer);
        return buffer;
    }
    
    
private:
    pVMObject resultActivation;
};

#endif
