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

#include "GlobalObjectId.h"

class Message {
public:
    Message(GlobalObjectId receiver, char* signature, size_t num_args, 
            GlobalObjectId* arguments)
    : receiver(receiver), signature(signature), number_of_arguments(num_args), 
      arguments(arguments) {}
    
private:
    GlobalObjectId receiver;
    char* signature; // message signature to be send to reviever object in remote actor
    size_t number_of_arguments;
    GlobalObjectId* arguments;
};

#endif
