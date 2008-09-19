#pragma once

#ifndef CORE_STRING_H_
#define CORE_STRING_H_

/*
 * $Id: String.h 109 2007-09-17 20:39:52Z tobias.pape $
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

class VMObject;
class VMFrame;
#include "Primitive.h"
class _String : Primitive
{
public:
    void  Concatenate_(VMObject* object, VMFrame* frame);
    void  AsSymbol(VMObject* object, VMFrame* frame);
    void  Hashcode(VMObject* object, VMFrame* frame);
    void  Length(VMObject* object, VMFrame* frame);
    void  Equal(VMObject* object, VMFrame* frame);
    void  PrimSubstringFrom_To_(VMObject* object, VMFrame* frame);
};


extern _String* String;

#endif CORE_STRING_H_

