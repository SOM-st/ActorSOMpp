/*
 *  RemoteObjectManager.h
 *  SOM
 *
 *  Created by Stefan Marr on 05/06/09.
 *  Copyright 2009 Vrije Universiteit Brussel. All rights reserved.
 *
 */

#include <map>

#include "GlobalObjectId.h"

#include "../vmobjects/VMRemoteObject.h"

//macro to access the object table
//#define _REMOTE_OBJECT_MANAGER RemoteObjectManager::GetRemoteObjectManager()

class RemoteObjectManager  {
public:
    // Singleton accessor
    /*static RemoteObjectManager& GetRemoteObjectManager() {
        if (!theRemoteObjectManager) {
            theRemoteObjectManager = new RemoteObjectManager();
        }
        return *theRemoteObjectManager;
    }*/
    
    static VMObject* GetObject(GlobalObjectId id) {
        if (objectMap[id] == NULL) {
            objectMap[id] = _UNIVERSE->NewRemoteObject(id);
        }
        return objectMap[id];
    }
    
    //static RemoteObjectManager* theRemoteObjectManager;
    
private:
    static std::map<GlobalObjectId, VMRemoteObject*> objectMap;
};