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
#include <vector>
#include <typeinfo>

#include "../misc/debug.h"


void ActorMessaging::ReceiveAndProcessMessages() {
    while (actors_msgbuffer_holds_data()) {
        void* buffer;
        size_t size;
        actors_msgbuffer_read_msg(&buffer, &size);
        
        Message* msg = Message::Deserialize(buffer);
#ifdef DEBUG
        DebugLog("ActorMessaging received msg: %s\n", typeid(*msg).name());
#endif  
        msg->Process();
        
        free(buffer);
        delete msg;
    }
}

void ActorMessaging::SendObjectReference(pVMObject obj, actor_id_t actorId) {
    ObjRefMessage msg(obj);
    SendMessage(&msg, actorId);
}

void ActorMessaging::SendMessage(Message* msg, actor_id_t actorId) {
    msg->TrackObjectSends(actorId);
    
    DebugLog("Send Msg: %s to %d\n", typeid(*msg).name(), actorId);
    size_t buffer_size = msg->GetSize();
  
    // this size alignment is for the syncedqueue implementation which uses int32_t as smallest units
    void* buffer = malloc( ((buffer_size / sizeof(int32_t)) + 1) * sizeof(int32_t) );
    
    msg->Serialize(buffer);
        
    actors_msgbuffer_send_msg(actorId, buffer, buffer_size);

    free(buffer);
    
//    assert(size % sizeof(int32_t) == 0); to make sure the allocated memory is alligned
}