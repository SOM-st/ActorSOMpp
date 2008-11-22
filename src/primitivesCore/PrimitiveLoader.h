#pragma once

#ifndef PRIMITIVESLOADER_H_
#define PRIMITIVESLOADER_H_
#include <map>
#include "../misc/defs.h"

class Primitive;
class PrimitiveRoutine;

class PrimitiveLoader {
public:
    PrimitiveLoader();
    virtual ~PrimitiveLoader();
    virtual PrimitiveRoutine* GetPrimitiveRoutine(const pString& cname, pString mname);
    virtual void AddPrimitiveObject(const char*, Primitive*);
    virtual bool SupportsClass(const char*);
private:
    map<pString, Primitive*> primitiveObjects;
};

#endif PRIMITIVESCORE_H_
