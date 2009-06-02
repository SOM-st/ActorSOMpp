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

#include "Message.h"

class ActorMessaging {
public:
 
  bool HasIncommingMessages();
  Message* ReceiveMessage();
  void SendMessage(Message* msg, actor_id_t actorId);
  
private:

};

#endif
