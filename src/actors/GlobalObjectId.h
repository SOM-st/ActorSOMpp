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
  ObjectTable::Index  index;
  actor_id_t          actor_id;
  
  inline int operator<(const GlobalObjectId o) const {
    if (actor_id < o.actor_id) 
      return true;
    
    return actor_id == o.actor_id && index.value < o.index.value;
  }
  
} GlobalObjectId;

#endif
