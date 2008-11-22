#pragma once

#ifndef PRIMITIVE_H_
#define PRIMITIVE_H_
#include "../misc/defs.h"
#include "../vmobjects/PrimitiveRoutine.h"

//abstract base class for all CPPSOM primitives
class Primitive {

public:
    Primitive();
    virtual PrimitiveRoutine* GetRoutine(const pString& routineName);
    virtual ~Primitive();
    virtual void SetRoutine(const char* name, PrimitiveRoutine* routine);

private:
    map<pString, PrimitiveRoutine*>* methods;
};

#endif
