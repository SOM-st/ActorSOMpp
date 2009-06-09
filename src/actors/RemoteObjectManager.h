/*
 *  RemoteObjectManager.h
 *  SOM
 *
 *  Created by Stefan Marr on 05/06/09.
 *  Copyright 2009 Vrije Universiteit Brussel. All rights reserved.
 *
 */

#include <map>

#include "../vm/Universe.h"

#include "GlobalObjectId.h"

#include "../vmobjects/VMRemoteObject.h"

//macro to access the object table
//#define _REMOTE_OBJECT_MANAGER RemoteObjectManager::GetRemoteObjectManager()

class RemoteObjectManager  {
public:    
    static pVMObject GetObject(GlobalObjectId id) {
        if (id.actor_id == actors_id() || id.index.is_iint)
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
    
private:
    static std::map<GlobalObjectId, VMRemoteObject*> objectMap;
};