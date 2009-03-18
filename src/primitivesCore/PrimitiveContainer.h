#pragma once

#ifndef PRIMITIVECONTAINER_H_
#define PRIMITIVECONTAINER_H_
#include "../misc/defs.h"
#include "../vmobjects/PrimitiveRoutine.h"
#include <map>

///Base class for all container objects holding SOM++ primitives.
//Primitive container classes need to initialize a std::map<StdString,
//PrimitiveRoutine*> in order to map smalltalk message names to the method
//to call.
class PrimitiveContainer {

public:
    PrimitiveContainer();
    virtual ~PrimitiveContainer();

    ///Every derived Class must use this method to initialize the methods
    //map with the mapping of a StdString with the smalltalk message
    //name and the corresponding functor object. The abstract functor object
    //class is defined in vmobjects/PrimitiveRoutine. Basically, the only
    //requirement for those objects is to implement:
    //  virtual void operator()(pVMObject, pVMFrame)
    virtual void SetPrimitive(const char* name, PrimitiveRoutine* routine);

    virtual PrimitiveRoutine* GetPrimitive(const std::string& routineName);

private:
    std::map<std::string, PrimitiveRoutine*>* methods;
};

#endif
