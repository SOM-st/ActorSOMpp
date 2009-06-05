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

SomMessage* ActorMessaging::ReceiveMessage() {
    void* buffer;
    size_t size;
    actors_msgbuffer_read_msg(&buffer, &size);
    
    Message* result;
    do {
        result = Message::Deserialize(buffer);
        result->Process();
        free(buffer);
    } while (result->GetType() != SOM_MSG);
    
    return (SomMessage*)result;
}

void ActorMessaging::SendMessage(SomMessage* msg, actor_id_t actorId) {
    size_t sig_len = strlen(msg->signature);
    size_t buffer_size = msg->GetSize();
  
    // this size alignment is for the syncedqueue implementation which uses int32_t as smallest units
    void* buffer = malloc( ((buffer_size / sizeof(int32_t)) + 1) * sizeof(int32_t) );
    
    msg->Serialize(buffer);
        
    actors_msgbuffer_send_msg(actorId, buffer, buffer_size);

    free(buffer);
    
//    assert(size % sizeof(int32_t) == 0); to make sure the allocated memory is alligned
}