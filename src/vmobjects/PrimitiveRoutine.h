#pragma once

#ifndef PRIMITIVEROUTINE_H_
#define PRIMITIVEROUTINE_H_

#include "VMObject.h"
#include "VMFrame.h"

// abstract base class
class PrimitiveRoutine {
public:
    PrimitiveRoutine() {  };

    virtual void operator()(pVMObject, pVMFrame)=0;  // call using operator
};

// Typedefs for Primitive loading
typedef PrimitiveRoutine* CreatePrimitive(const std::string&,const std::string&);
typedef bool SupportsClass(const char*);
typedef void TearDown();
typedef void Setup();

#endif
