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


std::vector<Message*> ActorMessaging::tempQueue;

bool ActorMessaging::HasIncommingMessages() {
    while (actors_msgbuffer_holds_data()) {
        ReceiveMessage(ANY_MESSAGE);
    }
    
    return !tempQueue.empty();
}

Message* ActorMessaging::CheckTempQueue(Messages msgType) {
    std::vector<Message*>::iterator it;
    
    for (it = tempQueue.begin(); it < tempQueue.end(); it++) {
        if ((*it)->GetType() & msgType) {
            Message* result = *it;
            tempQueue.erase(it);
            return result;
        }
    }
    return NULL;
}

Message* ActorMessaging::ReceiveMessage(Messages msgType) {
    Message* result = CheckTempQueue(msgType);
    if (result != NULL) {
        return result;
    }
    
    do {
        void* buffer;
        size_t size;
        actors_msgbuffer_read_msg(&buffer, &size);
        
        result = Message::Deserialize(buffer);
#ifdef DEBUG
        DebugLog("ActorMessaging received msg: %s\n", typeid(*result).name());
#endif  
        result->Process();
        if (result->GetType() != msgType && result->ShouldBeQueued()) {
            DebugLog("Message is temporary postponed\n");
            tempQueue.push_back(result);
        }
        
        free(buffer);        
    } while ((result->GetType() & msgType) == 0);
    DebugLog("Message will be handled now.\n");
    
    return result;
}

SomMessage* ActorMessaging::ReceiveSomMessage() { 
    return (SomMessage*)ReceiveMessage(SOM_MSG | SOM_MSG_WITH_RESULT);
}

pVMObject ActorMessaging::ReceiveObjectReference() {
    ObjRefMessage* msg = (ObjRefMessage*)ReceiveMessage(OBJ_REF_MSG);
    pVMObject obj = msg->GetObject();
    delete msg;
    return obj;
}

void ActorMessaging::SendObjectReference(pVMObject obj, actor_id_t actorId) {
    ObjRefMessage msg(RemoteObjectManager::GetGlobalId(obj));
    SendMessage(&msg, actorId);
}

void ActorMessaging::SendMessage(Message* msg, actor_id_t actorId) {
    DebugLog("Send Msg: %d to %d\n", msg->GetType(), actorId);
    size_t buffer_size = msg->GetSize();
  
    // this size alignment is for the syncedqueue implementation which uses int32_t as smallest units
    void* buffer = malloc( ((buffer_size / sizeof(int32_t)) + 1) * sizeof(int32_t) );
    
    msg->Serialize(buffer);
        
    actors_msgbuffer_send_msg(actorId, buffer, buffer_size);

    free(buffer);
    
//    assert(size % sizeof(int32_t) == 0); to make sure the allocated memory is alligned
}