/*
 *  VMRemoteObject.h
 *  SOM
 *
 *  Created by Stefan Marr on 05/06/09.
 *  Copyright 2009 Vrije Universiteit Brussel. All rights reserved.
 *
 */

#ifndef _VMREMOTE_OBJECT_H_
#define _VMREMOTE_OBJECT_H_

#include "../actors/GlobalObjectId.h"

#include "VMObject.h"


class VMRemoteObject : public VMObject {
public:
  VMRemoteObject(GlobalObjectId id)
  : VMObject(VMRemoteObjectNumberOfFields), globalId(id) {
  }
  
  inline GlobalObjectId GetGlobalId() const {
    return globalId;
  }
   
  virtual bool IsRemote() {
    return true;
  }
  
  virtual actor_id_t GetHomeId() {
    return globalId.actor_id;
  }
  
  
/*  inline bool operator==(VMRemoteObject o) const{
    return globalId.actor_id == o.globalId.actor_id && globalId.index.value == o.globalId.index.value;
  };*/
  
  virtual bool IsImmutable() {
    return true;
  }
  
  virtual ImmutableTypes GetSerializationTag() {
    return GlobalObjectIdTag;
  }
  
  
  virtual size_t GetSerializedSize() {
    return VMObject::GetSerializedSize()
    + sizeof(GlobalObjectId);
  }
  
  virtual void* Serialize(void* buffer) {
    buffer = VMObject::Serialize(buffer);
    
    *(GlobalObjectId*)buffer = globalId;
    
    return (void*)((intptr_t)buffer + sizeof(GlobalObjectId));
  }
  
  static void* Deserialize(void* buffer, GlobalObjectId& id) {
    id = *(GlobalObjectId*)buffer;
    
    return (void*)((intptr_t)buffer + sizeof(GlobalObjectId));
  }
  
  
private:
  GlobalObjectId globalId;
  
  static const int VMRemoteObjectNumberOfFields = 0;
};

#endif
