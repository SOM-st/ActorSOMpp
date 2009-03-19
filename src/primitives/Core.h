#pragma once

#ifndef CORE_H_
#define CORE_H_

//This header file is only necessary for Visual Studio compilation
#include "../primitivesCore/Routine.h"

extern "C" void setup();
extern "C" bool supportsClass(const char* name);
extern "C" void tearDown();
extern "C" PrimitiveRoutine* create(const StdString& cname, const StdString& fname);

#endif