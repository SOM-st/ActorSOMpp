/*
 *  ActorMessaging.h
 *  SOM
 *
 *  Created by Stefan Marr on 02/06/09.
 *  Copyright 2009 Vrije Universiteit Brussel. All rights reserved.
 *
 */

#ifndef _ACTOR_MESSAGING_H_
#define _ACTOR_MESSAGING_H_

#include "messages.h"

#include <vector>

class ActorMessaging {
public:
 
    static bool HasIncommingMessages();
    static SomMessage*  ReceiveSomMessage();
    static pVMObject    ReceiveObjectReference();
    
    static void SendMessage(Message* msg, actor_id_t actorId);
    static void SendObjectReference(pVMObject obj, actor_id_t actorId);
  
private:
    static Message* ReceiveMessage(Messages msgType);
    static Message* CheckTempQueue(Messages msgType);
    static std::vector<Message*> tempQueue;

};

#endif
