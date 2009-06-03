/*
 *  Message.h
 *  SOM
 *
 *  Created by Stefan Marr on 01/06/09.
 *  Copyright 2009 Vrije Universiteit Brussel. All rights reserved.
 *
 */

#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "../vmobjects/VMObject.h"
#include "../vmobjects/ObjectFormats.h"
#include "../memory/ObjectTable.h"

#include "GlobalObjectId.h"

class Message {
public:
    Message(GlobalObjectId receiver, char* signature, size_t num_args, 
            GlobalObjectId* arguments)
    : receiver(receiver), signature(signature), number_of_arguments(num_args), 
      arguments(arguments) {}
#warning missing deconstructor and memory leaks for signature and arguments array

    char* GetSignature() { return signature; }
    size_t GetNumberOfArguments() { return number_of_arguments; }
    pVMObject GetReceiver() { // receiver is always local, because message was already defered to the right actor
        return pVMObject(receiver.index);
    }
    
    pVMObject GetArgument(size_t i) {
        // give GlobalObjectId to objecttable, if its a local reference just return index, otherwise add it to the table an return new index
        return pVMObject(arguments[i]);
    }
    
    
private:
    GlobalObjectId receiver;
    char* signature; // message signature to be send to reviever object in remote actor
    size_t number_of_arguments;
    GlobalObjectId* arguments;
    
    friend class ActorMessaging;
};

#endif
