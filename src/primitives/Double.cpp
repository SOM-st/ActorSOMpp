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

#include "Double.h"

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
        _UNIVERSE->error_exit("Attempt to apply Double operation to non-number.");

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
    frame->Push((VMObject*)_UNIVERSE->new_double(left + right));
}


void  _Double::Minus(VMObject* /*object*/, VMFrame* frame) {
    PREPARE_OPERANDS;
    frame->Push((VMObject*)_UNIVERSE->new_double(left - right));
}


void  _Double::Star(VMObject* /*object*/, VMFrame* frame) {
    PREPARE_OPERANDS;
    frame->Push((VMObject*)_UNIVERSE->new_double(left * right));
}


void  _Double::Slashslash(VMObject* /*object*/, VMFrame* frame) {
    PREPARE_OPERANDS;
    frame->Push((VMObject*)_UNIVERSE->new_double(left / right));
}


void  _Double::Percent(VMObject* /*object*/, VMFrame* frame) {
    PREPARE_OPERANDS;
    frame->Push(_UNIVERSE->new_double((double)((int64_t)left % 
                                              (int64_t)right)));
}
void  _Double::And(VMObject* /*object*/, VMFrame* frame) {
    PREPARE_OPERANDS;
    frame->Push(_UNIVERSE->new_double((double)((int64_t)left & 
                                              (int64_t)right)));
}



/*
 * This function implements strict (bit-wise) equality and is therefore
 * inaccurate.
 */
void  _Double::Equal(VMObject* /*object*/, VMFrame* frame) {
    PREPARE_OPERANDS;
    if(left == right)
        frame->Push(true_object);
    else
        frame->Push(false_object);
}


void  _Double::Lowerthan(VMObject* /*object*/, VMFrame* frame) {
    PREPARE_OPERANDS;
    if(left < right)
        frame->Push(true_object);
    else
        frame->Push(false_object);
}


void  _Double::AsString(VMObject* /*object*/, VMFrame* frame) {
    VMDouble* self = (VMDouble*)frame->Pop();
    // temporary storage for the number string
    // use c99 snprintf-goodie
    double dbl = self->GetEmbeddedDouble();
    ostringstream Str;
    Str.precision(17);
    Str << dbl;
    frame->Push( (VMObject*)_UNIVERSE->new_string( pString(Str.str()) ) );
}


void _Double::Sqrt(VMObject* /*object*/, VMFrame* frame) {
    VMDouble* self = (VMDouble*)frame->Pop();
    VMDouble* result = _UNIVERSE->new_double( sqrt(self->GetEmbeddedDouble()) );
    frame->Push((VMObject*)result);
}
