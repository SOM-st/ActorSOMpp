/*
 *  RemoteObjectManager.cpp
 *  SOM
 *
 *  Created by Stefan Marr on 05/06/09.
 *  Copyright 2009 Vrije Universiteit Brussel. All rights reserved.
 *
 */

#include "RemoteObjectManager.h"

std::map<GlobalObjectId, VMRemoteObject*> RemoteObjectManager::objectMap;