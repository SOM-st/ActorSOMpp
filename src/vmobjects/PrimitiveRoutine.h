#pragma once

#ifndef PRIMITIVEROUTINE_H_
#define PRIMITIVEROUTINE_H_

#include "VMObject.h"
#include "VMFrame.h"


// abstract base class
class PrimitiveRoutine : public VMObject {
public:
    PrimitiveRoutine(int nof = 0) : VMObject(nof){  };
  // two possible functions to call member function. virtual cause derived
  // classes will use a pointer to an object and a pointer to a member function
  // to make the function call
  virtual void operator()(VMObject*, VMFrame*)=0;  // call using operator
  virtual void Call(VMObject*, VMFrame*)=0;        // call using function

  
  virtual void MarkReferences() {
      if (gcfield) return;
      //PrimitiveRoutine is a VMObject so it is allocated on the heap
      //but it doesn't have any fields not even a clazz, so no need 
      //to call VMObject::MarkReferences()
      this->SetGCField(1);
  }
};

// Typedefs for Primitive loading
typedef PrimitiveRoutine* CreatePrimitive(const pString&,const pString&);
typedef bool SupportsClass(const char*);
typedef void Setup();
typedef void TearDown();


#endif
