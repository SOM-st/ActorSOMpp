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
#include "../vmobjects/ObjectFormats.h"

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
  
  static size_t GetDirectSerializedSize() {
    return sizeof(GlobalObjectId);
  }
  
  size_t GetSerializedSize() {
    return sizeof(ImmutableTypes) + sizeof(GlobalObjectId);
  }
  
  void* SerializeDirect(void* buffer) {
    *(GlobalObjectId*)buffer = *this;
    
    return (void*)((intptr_t)buffer + sizeof(GlobalObjectId));
  }
  
  void* Serialize(void* buffer) {
    *(ImmutableTypes*)buffer = GlobalObjectIdTag;
    buffer = (void*)((intptr_t)buffer + sizeof(ImmutableTypes));
    
    buffer = SerializeDirect(buffer);
    
    return buffer;
  }
  
  static void* Deserialize(void* buffer, GlobalObjectId& id) {
    id = *(GlobalObjectId*)buffer;
    return (void*)((intptr_t)buffer + sizeof(GlobalObjectId));
  }
  
} GlobalObjectId;

#endif
