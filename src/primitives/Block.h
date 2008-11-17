#pragma once

#ifndef CORE_BLOCK_H_
#define CORE_BLOCK_H_

/*
 * $Id: Block.h 144 2007-11-19 23:32:02Z michael.haupt $
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

class _Block : public Primitive
{
public:
    virtual PrimitiveRoutine* GetRoutine(const pString& routineName);
    void  Value(VMObject* object, VMFrame* frame);
    void  Restart(VMObject* object, VMFrame* frame);
    void  Value_(VMObject* object, VMFrame* frame);
    void  Value_with_(VMObject* object, VMFrame* frame);
};

extern _Block* Block;

#endif
