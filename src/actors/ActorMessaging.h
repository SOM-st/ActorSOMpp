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
    
    static void ReceiveAndProcessMessages();
    
    static void SendMessage(Message* msg, actor_id_t actorId);
    static void SendObjectReference(pVMObject obj, actor_id_t actorId);
    
    // this one is not optimized, sends a lot to many messages, could combine them...
    static void NotifyOfNewReference(GlobalObjectId newlyReferencedObject, actor_id_t actorReceivingRef);
    static void NotifyOfUnlink(GlobalObjectId unlinkedObject);
};

#endif
