/*
 *  RemoteObjectManager.h
 *  SOM
 *
 *  Created by Stefan Marr on 05/06/09.
 *  Copyright 2009 Vrije Universiteit Brussel. All rights reserved.
 *
 */

#ifndef _REMOTE_OBJECT_MANAGER_H_
#define _REMOTE_OBJECT_MANAGER_H_

#include <map>
#include <bitset>

#include "../vm/Universe.h"

#include "GlobalObjectId.h"

#include "../vmobjects/VMRemoteObject.h"

class RemoteObjectManager  {
public:    
    static pVMObject GetObject(GlobalObjectId id) {
        if (actors_is_local(id.actor_id) || id.index.is_iint)
            return pVMObject(id.index);
        
        if (objectMap[id] == NULL) {
            objectMap[id] = _UNIVERSE->NewRemoteObject(id);
        }
        return objectMap[id];
    }
    
    static GlobalObjectId GetGlobalId(pVMObject obj) {
        if (obj->IsRemote()) {
            return ((pVMRemoteObject)obj)->GetGlobalId();
        }

        GlobalObjectId id;
        id.index = obj.GetIndex();
        id.actor_id = actors_id();
        return id;
    }
    
    static void TrackObjectSend(pVMObject obj, actor_id_t receivingActor);
    static void TrackObjectSend(GlobalObjectId id, actor_id_t receivingActor);
    static void NewExternalReference(ObjectTable::Index referencedObj,
                                     actor_id_t referencingActor);
    static void ObjectUnlinked(ObjectTable::Index unlinkedObj, actor_id_t actor); 
    
    static void UnlinkRemoteObject(GlobalObjectId unlinkedObj);
    
private:
    static std::map<GlobalObjectId, VMRemoteObject*> objectMap;
    
    typedef std::bitset<NUMBER_OF_ACTORS> ActorSet, *pActorSet;
    
    static std::map<ObjectTable::Index, pActorSet> externalReferences;
};

#endif
