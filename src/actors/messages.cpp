/*
 *  messages.cpp
 *  SOM
 *
 *  Created by Stefan Marr on 05/06/09.
 *  Copyright 2009 Vrije Universiteit Brussel. All rights reserved.
 *
 */

#include <err.h>

#include "messages.h"

#include "../vm/Universe.h"

Message* Message::Deserialize(void* buffer) {
    Messages msgType = (Messages)*(int32_t*)buffer;
    buffer = (void*)((intptr_t)buffer + sizeof(int32_t));
    
    switch (msgType) {
        case EXIT_MSG:       return new ExitMsg();
        case SOM_MSG:       return new SomMessage(buffer);
        default:
            warnx("Message::Deserialize: unexpected msgType\n");
    }
    
    return NULL;
}

void ExitMsg::Process() {
    _UNIVERSE->GetInterpreter()->Stop();
}


void SomMessage::Process() {}