/*
 *  GlobalObjectId.h
 *  SOM
 *
 *  Created by Stefan Marr on 02/06/09.
 *  Copyright 2009 Vrije Universiteit Brussel. All rights reserved.
 *
 */

#ifndef _GLOBAL_OBJECT_ID_H_
#define _GLOBAL_OBJECT_ID_H_

#include "actors.h"
#include "../memory/ObjectTable.h"

typedef struct GlobalObjectId {
  // no migration support for now
  // for migration it would be an idea to make it more to an global id
  // add a time stamp and add an actor_id and index which is stable and based on
  // creation and then use the curren index and actor_id for fast lookup which
  // can be updated on migration
  ObjectTable::Index  index;
  actor_id_t          actor_id;
  
  inline int operator<(const GlobalObjectId o) const {
    if (actor_id < o.actor_id) 
      return true;
    
    return actor_id == o.actor_id && index.value < o.index.value;
  }
  
} GlobalObjectId;

#endif
