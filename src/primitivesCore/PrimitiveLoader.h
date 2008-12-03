#pragma once

#ifndef PRIMITIVESLOADER_H_
#define PRIMITIVESLOADER_H_
#include <map>
#include "../misc/defs.h"

class PrimitiveContainer;
class PrimitiveRoutine;

///Core class for primitive loading.
//In order to implement new primitive libraries, you can use this class
//to implement the "create" factory method (of course you can do something
//totally different, if you chose...)
//
//Functions that are expected to be exported by the library are:
//bool supportsClass(const char* name)
//void tearDown()
//PrimitiveRoutine* create(const StdString& cname, const StdString& fname)
//
//The expected file extension is ".csp".
//Libraries have to take care of initializing any needed data or data structures.
//When using the PrimitiveLoader class that is the the std::map primitiveObjects.
//Initialize it by calling the AddPrimitiveObject method, in order to map the 
//name of the smalltalk class to the corresponding PrimitiveContainer object.
class PrimitiveLoader {
public:
    PrimitiveLoader();
    virtual ~PrimitiveLoader();
    virtual PrimitiveRoutine* 
                GetPrimitiveRoutine(const StdString& cname, const StdString& mname);
    virtual void AddPrimitiveObject(const char*, PrimitiveContainer*);
    virtual bool SupportsClass(const char*);
private:
    map<StdString, PrimitiveContainer*> primitiveObjects;
};

#endif PRIMITIVESCORE_H_
