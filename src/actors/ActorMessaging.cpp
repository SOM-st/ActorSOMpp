/*
 *  ActorMessaging.cpp
 *  SOM
 *
 *  Created by Stefan Marr on 02/06/09.
 *  Copyright 2009 Vrije Universiteit Brussel. All rights reserved.
 *
 */

#include "ActorMessaging.h"

#include <string.h>
#include <stdlib.h>

bool ActorMessaging::HasIncommingMessages() {
    return actors_msgbuffer_holds_data();
}

Message* ActorMessaging::ReceiveMessage() {
    void* buffer;
    size_t size;
    actors_msgbuffer_read_msg(&buffer, &size);
    
    GlobalObjectId* receiver = (GlobalObjectId*)buffer;
    
    size_t* signature_len = (size_t*)((intptr_t)buffer + sizeof(GlobalObjectId));
   
    char* signature = (char*)((intptr_t)signature_len + sizeof(size_t));
    
    size_t* num_args = (size_t*)((intptr_t)signature + *signature_len + 1);
    
    GlobalObjectId* arguments = (GlobalObjectId*)((intptr_t)num_args + sizeof(size_t));
    
    Message* result = new Message(*receiver, signature, *num_args, arguments);
    return result;
}

void ActorMessaging::SendMessage(Message* msg, actor_id_t actorId) {
    size_t sig_len = strlen(msg->signature);
    size_t buffer_size =  sizeof(GlobalObjectId)
                        + sizeof(size_t) + sig_len + 1
                        + sizeof(size_t)
                        + sizeof(GlobalObjectId) * msg->number_of_arguments;
  
    void* buffer = malloc( ((buffer_size / sizeof(int32_t)) + 1) * sizeof(int32_t) );
    GlobalObjectId* receiver = (GlobalObjectId*)buffer;
    *receiver = msg->receiver;
    
    size_t* signature_len = (size_t*)((intptr_t)buffer + sizeof(GlobalObjectId));
    *signature_len = sig_len;
    
    char* signature = (char*)((intptr_t)signature_len + sizeof(size_t));
    strncpy(signature, msg->signature, sig_len + 1);
    
    size_t* num_args = (size_t*)((intptr_t)signature + sig_len + 1);
    *num_args = msg->number_of_arguments;
                                  
    GlobalObjectId* arguments = (GlobalObjectId*)((intptr_t)num_args + sizeof(size_t));

    for (size_t i = 0; i < msg->number_of_arguments; i++) {
        *arguments = msg->arguments[i];
        arguments++;
    }
    
    actors_msgbuffer_send_msg(actorId, buffer, buffer_size);

    free(buffer);
    
//    assert(size % sizeof(int32_t) == 0); to make sure the allocated memory is alligned
}