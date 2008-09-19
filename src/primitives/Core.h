#pragma once

#ifndef CORE_H_
#define CORE_H_

class PrimitiveRoutine;
#include "../misc/defs.h"

class Core
{
public:
// the types of the class factories
static void setup();
static void tearDown();
static PrimitiveRoutine* create(const pString& cname, const pString& fname);
static void destroy(PrimitiveRoutine* rtn);
};


#endif
