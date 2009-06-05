/*
 *  VMRemoteObject.h
 *  SOM
 *
 *  Created by Stefan Marr on 05/06/09.
 *  Copyright 2009 Vrije Universiteit Brussel. All rights reserved.
 *
 */

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
  
/*  inline bool operator==(VMRemoteObject o) const{
    return globalId.actor_id == o.globalId.actor_id && globalId.index.value == o.globalId.index.value;
  };*/
  
private:
  GlobalObjectId globalId;
  
  static const int VMRemoteObjectNumberOfFields = 0;
};
