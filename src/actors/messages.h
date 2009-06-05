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
    
    EXIT_MSG,
    
    SOM_MSG
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
  pVMObject GetReceiver() { // receiver is always local, because message was already defered to the right actor
    return pVMObject(receiver.index);
  }
  
  pVMObject GetArgument(size_t i) {
    // give GlobalObjectId to objecttable, if its a local reference just return index, otherwise add it to the table an return new index
    return pVMObject(RemoteObjectManager::GetObject(arguments[i]));
  }
  
  virtual size_t GetSize() {
    return    sizeof(GlobalObjectId)
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

    return NULL;
  }
    
  virtual void Process();
  
private:
  GlobalObjectId receiver;
  char* signature; // message signature to be send to reviever object in remote actor
  size_t sig_len;
  size_t number_of_arguments;
  GlobalObjectId* arguments;
  
  friend class ActorMessaging;
};

#endif
