/*
 * $Id: Double.c 248 2008-04-28 07:33:27Z michael.haupt $
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

#include <stdio.h>
#include <iostream>
#include <math.h>
#include <sstream>
//#include "../memory/gc.h"

#include "../vmobjects/VMObject.h"
#include "../vmobjects/VMFrame.h"
#include "../vmobjects/VMDouble.h"
#include "../vmobjects/VMInteger.h"
#include "../vmobjects/VMBigInteger.h"

#include "../vm/Universe.h"
#include "Core.h"
#include "Double.h"
#include "Routine.h"
_Double* Double;
/*
 * This function coerces any right-hand parameter to a double, regardless of its
 * true nature. This is to make sure that all Double operations return Doubles.
 */
double coerce_double(VMObject* x) {
    if(dynamic_cast<VMDouble*>(x) != NULL)
        return ((VMDouble*)x)->GetEmbeddedDouble();
    else if(dynamic_cast<VMInteger*>(x) != NULL)
        return (double)((VMInteger*)x)->GetEmbeddedInteger();
    else if(dynamic_cast<VMBigInteger*>(x) != NULL)
        return (double)((VMBigInteger*)x)->GetEmbeddedInteger();
    else
        universe->ErrorExit("Attempt to apply Double operation to non-number.");

    return 0.0f;
}


/*
 * The following standard functionality is performed in all arithmetic
 * operations: extract the right-hand operand by coercing it to a double, and
 * extract the left-hand operand as an immediate Double. Afterwards, left and
 * right are prepared for the operation.
 */
#define PREPARE_OPERANDS \
    double right = coerce_double(frame->Pop()); \
    VMDouble* leftObj = (VMDouble*)frame->Pop(); \
    double left = leftObj->GetEmbeddedDouble();


void  _Double::Plus(VMObject* /*object*/, VMFrame* frame) {
    PREPARE_OPERANDS;
    frame->Push((VMObject*)universe->NewDouble(left + right));
}


void  _Double::Minus(VMObject* /*object*/, VMFrame* frame) {
    PREPARE_OPERANDS;
    frame->Push((VMObject*)universe->NewDouble(left - right));
}


void  _Double::Star(VMObject* /*object*/, VMFrame* frame) {
    PREPARE_OPERANDS;
    frame->Push((VMObject*)universe->NewDouble(left * right));
}


void  _Double::Slashslash(VMObject* /*object*/, VMFrame* frame) {
    PREPARE_OPERANDS;
    frame->Push((VMObject*)universe->NewDouble(left / right));
}


void  _Double::Percent(VMObject* /*object*/, VMFrame* frame) {
    PREPARE_OPERANDS;
    frame->Push(universe->NewDouble((double)((int64_t)left % 
                                              (int64_t)right)));
}
void  _Double::And(VMObject* /*object*/, VMFrame* frame) {
    PREPARE_OPERANDS;
    frame->Push(universe->NewDouble((double)((int64_t)left & 
                                              (int64_t)right)));
}



/*
 * This function implements strict (bit-wise) equality and is therefore
 * inaccurate.
 */
void  _Double::Equal(VMObject* /*object*/, VMFrame* frame) {
    PREPARE_OPERANDS;
    if(left == right)
        frame->Push(Globals::TrueObject());
    else
        frame->Push(Globals::FalseObject());
}


void  _Double::Lowerthan(VMObject* /*object*/, VMFrame* frame) {
    PREPARE_OPERANDS;
    if(left < right)
        frame->Push(Globals::TrueObject());
    else
        frame->Push(Globals::FalseObject());
}


void  _Double::AsString(VMObject* /*object*/, VMFrame* frame) {
    VMDouble* self = (VMDouble*)frame->Pop();
    // temporary storage for the number string
    // use c99 snprintf-goodie
    double dbl = self->GetEmbeddedDouble();
    ostringstream Str;
    Str.precision(17);
    Str << dbl;
    frame->Push( (VMObject*)universe->NewString( pString(Str.str()) ) );
}


void _Double::Sqrt(VMObject* /*object*/, VMFrame* frame) {
    VMDouble* self = (VMDouble*)frame->Pop();
    VMDouble* result = universe->NewDouble( sqrt(self->GetEmbeddedDouble()) );
    frame->Push((VMObject*)result);
}

PrimitiveRoutine* _Double::GetRoutine( const pString& routineName )
{
    PrimitiveRoutine* result;
    if (routineName == pString("Plus"))
        result = new (heap) Routine<_Double>(Double, &_Double::Plus);
    else if (routineName == pString("Minus"))
        result = new (heap) Routine<_Double>(Double, &_Double::Minus);
    else if (routineName == pString("Star"))
        result = new (heap) Routine<_Double>(Double, &_Double::Star);
    else if (routineName == pString("Slashslash"))
        result = new (heap) Routine<_Double>(Double, &_Double::Slashslash);
    else if (routineName == pString("Percent"))
        result = new (heap) Routine<_Double>(Double, &_Double::Percent);
    else if (routineName == pString("And"))
        result = new (heap) Routine<_Double>(Double, &_Double::And);
    else if (routineName == pString("Equal"))
        result = new (heap) Routine<_Double>(Double, &_Double::Equal);
    else if (routineName == pString("Lowerthan"))
        result = new (heap) Routine<_Double>(Double, &_Double::Lowerthan);
    else if (routineName == pString("AsString"))
        result = new (heap) Routine<_Double>(Double, &_Double::AsString);
    else if (routineName == pString("Sqrt"))
        result = new (heap) Routine<_Double>(Double, &_Double::Sqrt);
    else {
        cout << "method " << routineName << "not found in class Double" << endl;
        return NULL;
    }
    return result;
}

