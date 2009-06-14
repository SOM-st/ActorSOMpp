/*
 *  RemoteObjectManager.cpp
 *  SOM
 *
 *  Created by Stefan Marr on 05/06/09.
 *  Copyright 2009 Vrije Universiteit Brussel. All rights reserved.
 *
 */

#include "RemoteObjectManager.h"
#include "ActorMessaging.h"
#include "actors.h"

std::map<GlobalObjectId, VMRemoteObject*> RemoteObjectManager::objectMap;
std::map<ObjectTable::Index, RemoteObjectManager::pActorSet> RemoteObjectManager::externalReferences;


void RemoteObjectManager::TrackObjectSend(pVMObject obj, actor_id_t actorId) {
    // not interested in immutables or stuff send just local
    if (obj->IsImmutable() || actors_is_local(actorId))
        return;
        
    if (obj->IsRemote()) {
        // if it is not going home we have to notivy the owner about a new reference
        if (obj->GetHomeId() != actorId) {
            // this should be implicitly sound, since we do already have 
            // a registered reference to this object it should not be deleted
            ActorMessaging::NotifyOfNewReference(obj, actorId);
        }
    } else {
        // remember that actor has a reference to the object we own
        (*externalReferences[obj.GetIndex()])[actorId] = true;
    }
}

void RemoteObjectManager::TrackObjectSend(GlobalObjectId id, actor_id_t actorId) {
    // not interested in send just local
    if (actors_is_local(actorId))
        return;
    
    if (actors_is_local(id.actor_id)) {
        (*externalReferences[id.index])[actorId] = true;
    }
}



