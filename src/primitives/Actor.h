/*
 *  Actor.h
 *  SOM
 *
 *  Created by Stefan Marr on 06/06/09.
 *  Copyright 2009 Vrije Universiteit Brussel. All rights reserved.
 *
 */

#include "../primitivesCore/PrimitiveContainer.h"

class _Actor : public PrimitiveContainer
  {
  public:
    _Actor();
    void Id(pVMObject, pVMFrame frame);
    void NumberOfActors(pVMObject, pVMFrame frame);
  };