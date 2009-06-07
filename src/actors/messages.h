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

enum Messages {
    ABSTRACT_MSG,
    
    EXIT_MSG,  // ask an actor to stop after all incomming messages have been processed
    
    OBJ_REF_MSG, // message contains a reference to an object
    SOM_MSG // represents a message sent on language level which has to be send async to an actor
};



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
    virtual bool ShouldBeQueued() = 0;
    
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
    virtual bool ShouldBeQueued();
};



class ObjRefMessage : public Message {
public:
    ObjRefMessage(GlobalObjectId obj) : Message(OBJ_REF_MSG), object(obj) {}
    ObjRefMessage(void* buffer)
    : Message(OBJ_REF_MSG), object(*(GlobalObjectId*)buffer) { }
    
    virtual void* Serialize(void* buffer) {
        buffer = Message::Serialize(buffer);
        GlobalObjectId* objectRef = (GlobalObjectId*)buffer;
        *objectRef = this->object;
        objectRef++;
        return (void*)objectRef;
    }
    
    pVMObject GetObject() {
        // give GlobalObjectId to objecttable, 
        // if its a local reference just return index,
        // otherwise add it to the table an return new index
        return pVMObject(RemoteObjectManager::GetObject(object));
    }
    
    virtual void Process() {}
    virtual bool ShouldBeQueued() { return true; }
    
    virtual size_t GetSize() {
        return Message::GetSize() + sizeof(GlobalObjectId);
    }
    
private:
    GlobalObjectId object;
};



class SomMessage : public Message {
public:
  SomMessage(GlobalObjectId receiver, char* signature, size_t num_args, 
             GlobalObjectId* arguments)
  : Message(SOM_MSG), receiver(receiver), signature(signature), number_of_arguments(num_args), 
  arguments(arguments) {
    sig_len = strlen(signature);
  }
  
  SomMessage(void* buffer) : Message(SOM_MSG) {
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
  }
#warning missing deconstructor and memory leaks for signature and arguments array
  
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
  virtual bool ShouldBeQueued() { return true; }
  
private:
  GlobalObjectId receiver;
  char* signature; // message signature to be send to reviever object in remote actor
  size_t sig_len;
  size_t number_of_arguments;
  GlobalObjectId* arguments;
};

#endif
