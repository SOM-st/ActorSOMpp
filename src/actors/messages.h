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
};



class ObjRefMessage : public Message {
public:
    ObjRefMessage(GlobalObjectId obj) : Message(OBJ_REF_MSG), object(obj) {}
    ObjRefMessage(Messages msgType, GlobalObjectId obj) : object(obj), Message(msgType) {}
    ObjRefMessage(void* buffer) : Message(OBJ_REF_MSG) { Deserialize(buffer); }
    ObjRefMessage(Messages msgType) : Message(msgType) {}
    
    virtual void* Serialize(void* buffer) {
        buffer = Message::Serialize(buffer);
        GlobalObjectId* objectRef = (GlobalObjectId*)buffer;
        *objectRef = this->object;
        objectRef++;
        return (void*)objectRef;
    }
    
    virtual void* Deserialize(void* buffer) {
        object = *(GlobalObjectId*)buffer;
        return (void*) ((intptr_t)buffer + sizeof(GlobalObjectId));
    }
    
    pVMObject GetObject() {
        // give GlobalObjectId to objecttable, 
        // if its a local reference just return index,
        // otherwise add it to the table an return new index
        return RemoteObjectManager::GetObject(object);
    }
    
    virtual void Process();
    
    virtual size_t GetSize() {
        return Message::GetSize() + sizeof(GlobalObjectId);
    }
    
private:
    GlobalObjectId object;
};



class ResultObjRefMessage : public ObjRefMessage {
public:
    ResultObjRefMessage(GlobalObjectId result, GlobalObjectId activation)
    : ObjRefMessage(OBJ_REF_RESULT_MSG, result), resultActivation(activation) {}
    
    ResultObjRefMessage(void* buffer) : ObjRefMessage(OBJ_REF_MSG) { 
        Deserialize(buffer); 
    }
    
    virtual void* Serialize(void* buffer) {
        buffer = ObjRefMessage::Serialize(buffer);
        GlobalObjectId* resultAct = (GlobalObjectId*)buffer;
        *resultAct = resultActivation;
        resultAct++;
        return (void*)resultAct;
    }
    
    virtual void* Deserialize(void* buffer) {
        buffer = ObjRefMessage::Deserialize(buffer);
        resultActivation = *(GlobalObjectId*)buffer;
        return (void*) ((intptr_t)buffer + sizeof(GlobalObjectId));
    }
    
    virtual void Process();
    
    virtual size_t GetSize() {
        return ObjRefMessage::GetSize() + sizeof(GlobalObjectId);
    }
    
private:
    GlobalObjectId resultActivation;    
};



class SomMessage : public Message {
public:
    SomMessage(Messages msgType, GlobalObjectId receiver, char* signature,
               size_t num_args, GlobalObjectId* arguments)
    : Message(msgType), receiver(receiver), signature(signature), 
      number_of_arguments(num_args), arguments(arguments) {
        sig_len = strlen(signature);
    }  
    
    SomMessage(GlobalObjectId receiver, char* signature, size_t num_args, 
               GlobalObjectId* arguments)
    : Message(SOM_MSG), receiver(receiver), signature(signature), 
      number_of_arguments(num_args), arguments(arguments) {
        sig_len = strlen(signature);
    }
  
    SomMessage(void* buffer) : Message(SOM_MSG) {
        Deserialize(buffer);
    }
    
    SomMessage(Messages msgType) : Message(msgType) {}

    #warning missing deconstructor and memory leaks for signature and arguments array
    virtual void* Deserialize(void* buffer) {
        GlobalObjectId* receiver = (GlobalObjectId*)buffer;
        
        size_t* signature_len = (size_t*)((intptr_t)buffer + sizeof(GlobalObjectId));
        
        char* signature = (char*)((intptr_t)signature_len + sizeof(size_t));
        
        size_t* num_args = (size_t*)((intptr_t)signature + *signature_len + 1);
        
        GlobalObjectId* arguments = (GlobalObjectId*)((intptr_t)num_args + sizeof(size_t));
        
        
        this->receiver = *receiver;
        this->signature = strdup(signature);
        this->sig_len = *signature_len;
        this->number_of_arguments = *num_args;
        this->arguments = (GlobalObjectId*)malloc(sizeof(GlobalObjectId) * *num_args);
        memcpy(this->arguments, arguments, sizeof(GlobalObjectId) * *num_args);

        return (void*)((intptr_t)arguments + sizeof(GlobalObjectId) * *num_args);
    }
    
    char* GetSignature() { return signature; }
    size_t GetNumberOfArguments() { return number_of_arguments; }
    pVMObject GetReceiver() { 
      // receiver is always local, because message was already defered to the right actor
    #warning does that hold when object migration is allowed?
        return pVMObject(receiver.index);
    }
  
    pVMObject GetArgument(size_t i) {
        // give GlobalObjectId to objecttable, if its a local reference just return index, otherwise add it to the table an return new index
        return pVMObject(RemoteObjectManager::GetObject(arguments[i]));
    }
  
    virtual size_t GetSize() {
        return Message::GetSize()
              + sizeof(GlobalObjectId)
              + sizeof(size_t) + sig_len + 1
              + sizeof(size_t)
              + sizeof(GlobalObjectId) * this->number_of_arguments;
    }
  
    virtual void* Serialize(void* buffer) {
        buffer = Message::Serialize(buffer);
        GlobalObjectId* receiver = (GlobalObjectId*)buffer;
        *receiver = this->receiver;

        size_t* signature_len = (size_t*)((intptr_t)buffer + sizeof(GlobalObjectId));
        *signature_len = sig_len;

        char* signature = (char*)((intptr_t)signature_len + sizeof(size_t));
        strncpy(signature, this->signature, sig_len + 1);

        size_t* num_args = (size_t*)((intptr_t)signature + sig_len + 1);
        *num_args = this->number_of_arguments;

        GlobalObjectId* arguments = (GlobalObjectId*)((intptr_t)num_args + sizeof(size_t));

        for (size_t i = 0; i < this->number_of_arguments; i++) {
            *arguments = this->arguments[i];
            arguments++;
        }        

        return (void*)arguments;
    }

    virtual void Process();
  
private:
    GlobalObjectId receiver;
    char* signature; // message signature to be send to reviever object in remote actor
    size_t sig_len;
    size_t number_of_arguments;
    GlobalObjectId* arguments;
};

class SomMessageWithResult : public SomMessage {
public:
    SomMessageWithResult(GlobalObjectId receiver, char* signature, size_t num_args, 
               GlobalObjectId* arguments, GlobalObjectId resultActivation)
    : SomMessage(SOM_MSG_WITH_RESULT, receiver, signature, num_args, arguments),
      resultActivation(resultActivation) {}
    
    SomMessageWithResult(void* buffer) : SomMessage(SOM_MSG_WITH_RESULT) {
        Deserialize(buffer);
    }
    
#warning missing deconstructor and memory leaks for signature and arguments array
    virtual void* Deserialize(void* buffer) {
        buffer = SomMessage::Deserialize(buffer);
        GlobalObjectId* resultActivation = (GlobalObjectId*)buffer;
        
        this->resultActivation = *resultActivation;
        return (void*)(resultActivation + 1);
    }
    
    
    virtual size_t GetSize() {
        return SomMessage::GetSize()
              + sizeof(GlobalObjectId);
    }
    
    virtual void* Serialize(void* buffer) {
        buffer = SomMessage::Serialize(buffer);
        GlobalObjectId* resultActivation = (GlobalObjectId*)buffer;
        *resultActivation = this->resultActivation;
        
        return (void*)((intptr_t)resultActivation + sizeof(GlobalObjectId));
    }
    
    virtual void Process();
    
    pVMObject GetResultActivation() {
        return RemoteObjectManager::GetObject(resultActivation);
    }
    
private:
    GlobalObjectId resultActivation;
};

#endif
