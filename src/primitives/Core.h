#pragma once

#ifndef CORE_H_
#define CORE_H_
#include "../primitivesCore/Routine.h"
extern "C" void setup();
extern "C" bool supportsClass(const char* name);
extern "C" void tearDown();
extern "C" PrimitiveRoutine* create(const StdString& cname, const StdString& fname);

#endif