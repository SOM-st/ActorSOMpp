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

#include <assert.h>

std::map<GlobalObjectId, VMRemoteObject*> RemoteObjectManager::objectMap;
std::map<ObjectTable::Index, RemoteObjectManager::pActorSet> RemoteObjectManager::externalReferences;


void RemoteObjectManager::TrackObjectSend(pVMObject obj, actor_id_t receivingActor) {
    // not interested in immutables or stuff send just local
    if (obj->IsImmutable() || actors_is_local(receivingActor))
        return;
        
    if (obj->IsRemote()) {
        // if it is not going home we have to notivy the owner about a new reference
        if (obj->GetHomeId() != receivingActor) {
            // this should be implicitly sound, since we do already have 
            // a registered reference to this object it should not be deleted
            ActorMessaging::NotifyOfNewReference(((pVMRemoteObject)obj)->GetGlobalId(), receivingActor);
        }
    } else {
        // remember that actor has a reference to the object we own
        NewExternalReference(obj.GetIndex(), receivingActor);
    }
}

void RemoteObjectManager::TrackObjectSend(GlobalObjectId id, actor_id_t receivingActor) {
    // not interested in send just local
    if (actors_is_local(receivingActor))
        return;
    
    if (actors_is_local(id.actor_id)) {
        NewExternalReference(id.index, receivingActor);
    } else if (id.actor_id == receivingActor) {
        // this should be implicitly sound, since we do already have 
        // a registered reference to this object it should not be deleted
        ActorMessaging::NotifyOfNewReference(id, receivingActor);
    }
}

void RemoteObjectManager::NewExternalReference(ObjectTable::Index referencedObj,
                                               actor_id_t referencingActor) {
    if (!externalReferences[referencedObj]) {
        externalReferences[referencedObj] = new ActorSet();
    }
    (*externalReferences[referencedObj])[referencingActor] = true;
}

void RemoteObjectManager::ObjectUnlinked(ObjectTable::Index unlinkedObj, actor_id_t actor) {
    assert(externalReferences[unlinkedObj]);
    (*externalReferences[unlinkedObj])[actor] = false;
    
    if (externalReferences[unlinkedObj]->none()) {
        delete externalReferences[unlinkedObj];
        externalReferences.erase(unlinkedObj);
    }
}

void RemoteObjectManager::UnlinkRemoteObject(GlobalObjectId unlinkedObj) {
    objectMap.erase(unlinkedObj);
    ActorMessaging::NotifyOfUnlink(unlinkedObj);
}

