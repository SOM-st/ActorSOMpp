/*
 * $Id: Array.c 223 2008-04-21 11:48:41Z michael.haupt $
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

#include <vmobjects/VMInteger.h>
#include <vmobjects/VMArray.h>
#include <vmobjects/VMObject.h>
#include <vmobjects/VMFrame.h>

#include <vm/Universe.h>
#include "Core.h"
#include "Array.h"
#include "Routine.h"
_Array* Array;

void _Array::At_(VMObject* /*object*/, VMFrame* frame) {
    VMInteger* index = (VMInteger*) frame->Pop();
    VMArray* self = (VMArray*) frame->Pop();
    int i = index->GetEmbeddedInteger();
    VMObject* elem = (*self)[i-1];//->GetIndexableField(i-1);
    frame->Push(elem);
}


void _Array::At_Put_(VMObject* /*object*/, VMFrame* frame) {
    VMObject* value = frame->Pop();
    VMInteger* index = (VMInteger*)frame->Pop();
    VMArray* self = (VMArray*)frame->GetStackElement(0);
    int i = index->GetEmbeddedInteger();
    self->SetIndexableField(i - 1, value);
}


void _Array::Length(VMObject* /*object*/, VMFrame* frame) {
    VMArray* self = (VMArray*) frame->Pop();
    VMInteger* new_int =
        universe->NewInteger(self->GetNumberOfIndexableFields());
    frame->Push((VMObject*)new_int);
}


void _Array::New_(VMObject* /*object*/, VMFrame* frame) {
    VMInteger* length = (VMInteger*)frame->Pop();
    /*VMClass* self = (VMClass*)*/
    frame->Pop();        
    int size = length->GetEmbeddedInteger();
    frame->Push((VMObject*) universe->NewArray(size));
}

PrimitiveRoutine* _Array::GetRoutine( const pString& routineName )
{
    PrimitiveRoutine* result;
    if (routineName == pString("New_"))
        result = new (heap) Routine<_Array>(Array, &_Array::New_);
    else if (routineName == pString("At_"))
        result = new (heap) Routine<_Array>(Array, &_Array::At_);
    else if (routineName == pString("At_put_"))
        result = new (heap) Routine<_Array>(Array, &_Array::At_Put_);
    else if (routineName == pString("Length"))
        result = new (heap) Routine<_Array>(Array, &_Array::Length);
    else {
        cout << "method " << routineName << " not found in class Array" << endl;
        return NULL;
    }
    return result;
}

