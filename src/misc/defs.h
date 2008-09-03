#pragma once
#ifndef DEFS_H_
#define DEFS_H_

/*
 * $Id: defs.h 111 2007-09-18 09:21:40Z tobias.pape $
 *
Copyright (c) 2007 Michael Haupt, Tobias Pape
Software Architecture Group, Hasso Plattner Institute, Potsdam, Germany
http://www.hpi.uni-potsdam.de/swa/

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 */

//#include <stdint.h>
#include <string>
#include <list>
#include <vector>
class VMObject;
#include "ExtendedList.h"
//
// error codes
//


#define ERR_SUCCESS        0x0
#define ERR_FAIL           0x1
#define ERR_NOMEM          0x2
#define ERR_PANIC          0xFFFF

#ifndef INT32_MAX
    /*Maximum value of 32-bit integer is 0x7FFF FFFF (2 147 483 647)         */
  #define INT32_MAX 0x7FFFFFFF  /*2 147 483 647*/
#endif
//
// macro for freeing an array
//
using namespace std;
#define path_separator '\\'
#define file_separator '\\'

//typedef int MethodGenerationContext;
//typedef int class_generation_context;
typedef string pVMSymbol;
typedef string pString;
typedef unsigned long long uint64_t;
typedef long long int64_t;
typedef unsigned long uint32_t;
typedef long int32_t, pVMInteger;
typedef unsigned int uint16_t;
typedef int int16_t;
typedef unsigned short uint8_t;
typedef short int8_t;
typedef ExtendedList<VMObject*> pList;


#endif DEFS_H_
