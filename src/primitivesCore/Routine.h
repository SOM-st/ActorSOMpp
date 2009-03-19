#pragma once

#ifndef ROUTINE_H_
#define ROUTINE_H_

#include <vmobjects/PrimitiveRoutine.h>

///Implementation for a functor class with PrimitiveRoutine as base class.
//It stores an object and a pointer to one of its methods. It is invoked
//by calling the Routine's poerator "()".
template <class TClass> class Routine : public PrimitiveRoutine
{
private:
  void (TClass::*fpt)(pVMObject, pVMFrame);   // pointer to member function
  TClass* pt2Object;                          // pointer to object

public:

  // constructor - takes pointer to an object and pointer to a member and stores
  // them in two private variables
   Routine(TClass* _pt2Object, void(TClass::*_fpt)(pVMObject, pVMFrame)) : 
       PrimitiveRoutine() { 
       pt2Object = _pt2Object;  
       fpt=_fpt; 
   };

  // override operator "()"
  virtual void operator()(pVMObject obj, pVMFrame frm) {
      (*pt2Object.*fpt)(obj, frm);// execute member function
  };              

};

#endif
