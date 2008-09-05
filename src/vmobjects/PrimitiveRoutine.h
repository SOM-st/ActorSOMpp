#pragma once

#ifndef PRIMITIVEROUTINE_H_
#define PRIMITIVEROUTINE_H_

#include "VMObject.h"
#include "VMFrame.h"

// abstract base class
class PrimitiveRoutine //: public VMObject
{
public:
    PrimitiveRoutine(){};// : VMObject() {};
  // two possible functions to call member function. virtual cause derived
  // classes will use a pointer to an object and a pointer to a member function
  // to make the function call
  virtual void operator()(VMObject*, VMFrame*)=0;  // call using operator
  virtual void Call(VMObject*, VMFrame*)=0;        // call using function
};


#endif
