/*
 *  Actor.cpp
 *  SOM
 *
 *  Created by Stefan Marr on 06/06/09.
 *  Copyright 2009 Vrije Universiteit Brussel. All rights reserved.
 *
 */

#include "Actor.h"

#include "../actors/ActorMessaging.h"

#include "../primitivesCore/Routine.h"

_Actor::_Actor() : PrimitiveContainer()
{
  this->SetPrimitive("id", static_cast<PrimitiveRoutine*>(
                     new Routine<_Actor>(this, &_Actor::Id)));
  this->SetPrimitive("numberOfActors", static_cast<PrimitiveRoutine*>(
                     new Routine<_Actor>(this, &_Actor::NumberOfActors)));
  this->SetPrimitive("receive", static_cast<PrimitiveRoutine*>(
                     new Routine<_Actor>(this, &_Actor::Receive)));
  this->SetPrimitive("send_toActor_", static_cast<PrimitiveRoutine*>(
                     new Routine<_Actor>(this, &_Actor::Send_toActor_)));
}

void _Actor::Id(pVMObject /*object*/, pVMFrame frame) {
  frame->Pop();
  
  frame->Push((pVMObject)_UNIVERSE->NewInteger(actors_id()));
}

void _Actor::NumberOfActors(pVMObject /*object*/, pVMFrame frame) {
  frame->Pop();
  
  frame->Push((pVMObject)_UNIVERSE->NewInteger(NUMBER_OF_ACTORS));
}

void _Actor::Receive(pVMObject /*object*/, pVMFrame frame) {
  frame->Pop();
  
  frame->Push(ActorMessaging::ReceiveObjectReference());
}

void _Actor::Send_toActor_(pVMObject, pVMFrame frame) {
  pVMInteger actorId = (pVMInteger)frame->Pop();
  pVMObject obj = frame->Pop();
  
  ActorMessaging::SendObjectReference(obj, actorId->GetEmbeddedInteger());
}
