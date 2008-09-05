/*
 * $Id: BigInteger.c 210 2008-04-16 12:54:12Z michael.haupt $
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

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sstream> 
//#include "../memory/gc.h"

#include "../vmobjects/VMObject.h"
#include "../vmobjects/VMFrame.h"
#include "../vmobjects/VMInteger.h"
#include "../vmobjects/VMBigInteger.h"

#include "../vm/Universe.h"


#include "BigInteger.h"

_BigInteger* BigInteger;

#define CHECK_BIGINT(object, result) { \
    /* Check second parameter type: */ \
    VMInteger* tmp;\
    if((tmp = dynamic_cast<VMInteger*>(object)) != NULL) { \
        /* Second operand was Integer*/ \
        int32_t i = tmp->GetEmbeddedInteger(); \
        (result) = _UNIVERSE->new_biginteger((int64_t)i); \
    } else \
        (result) = (VMBigInteger*)(object); \
}

#define PUSH_INT_OR_BIGINT(result) { \
    if(labs((result)) > INT32_MAX) \
        frame->Push((VMObject*)_UNIVERSE->new_biginteger((result))); \
    else \
        frame->Push((VMObject*)_UNIVERSE->new_integer((int32_t)(result))); \
}

void  _BigInteger::Plus(VMObject* object, VMFrame* frame) {
    VMObject* rightObj  = frame->Pop();
    VMBigInteger* right = NULL;
    VMBigInteger* left  = (VMBigInteger*)frame->Pop();
    
    CHECK_BIGINT(rightObj, right);
    
    // Do operation and perform conversion to Integer if required
    int64_t result = left->GetEmbeddedInteger()
                    + right->GetEmbeddedInteger();
    PUSH_INT_OR_BIGINT(result);
}


void  _BigInteger::Minus(VMObject* object, VMFrame* frame) {
    VMObject* rightObj  = frame->Pop();
    VMBigInteger* right = NULL;
    VMBigInteger* left  = (VMBigInteger*)frame->Pop();
    
    CHECK_BIGINT(rightObj, right);   
    
    // Do operation and perform conversion to Integer if required
    int64_t result =  left->GetEmbeddedInteger()
                    - right->GetEmbeddedInteger();
    PUSH_INT_OR_BIGINT(result);
}


void  _BigInteger::Star(VMObject* object, VMFrame* frame) {
   VMObject* rightObj  = frame->Pop();
    VMBigInteger* right = NULL;
    VMBigInteger* left  = (VMBigInteger*)frame->Pop();
    
    CHECK_BIGINT(rightObj, right);   
    
    // Do operation and perform conversion to Integer if required
    int64_t result =  left->GetEmbeddedInteger()
                    * right->GetEmbeddedInteger();
    PUSH_INT_OR_BIGINT(result);
}


void  _BigInteger::Slash(VMObject* object, VMFrame* frame) {
    VMObject* rightObj  = frame->Pop();
    VMBigInteger* right = NULL;
    VMBigInteger* left  = (VMBigInteger*)frame->Pop();
    
    CHECK_BIGINT(rightObj, right);   
    
    // Do operation and perform conversion to Integer if required
    int64_t result =  left->GetEmbeddedInteger()
                    / right->GetEmbeddedInteger();
    PUSH_INT_OR_BIGINT(result);
}


void  _BigInteger::Percent(VMObject* object, VMFrame* frame) {
    VMObject* rightObj  = frame->Pop();
    VMBigInteger* right = NULL;
    VMBigInteger* left  = (VMBigInteger*)frame->Pop();
    
    CHECK_BIGINT(rightObj, right);   
    
    // Do operation and perform conversion to Integer if required
    VMBigInteger* result = _UNIVERSE->new_biginteger(  left->GetEmbeddedInteger()
                                                    % right->GetEmbeddedInteger());
    
    frame->Push((VMObject*) result);
}


void  _BigInteger::And(VMObject* object, VMFrame* frame) {
    VMObject* rightObj  = frame->Pop();
    VMBigInteger* right = NULL;
    VMBigInteger* left  = (VMBigInteger*)frame->Pop();
    
    CHECK_BIGINT(rightObj, right);   
    
    // Do operation and perform conversion to Integer if required
    VMBigInteger* result = _UNIVERSE->new_biginteger(  left->GetEmbeddedInteger()
                                                    & right->GetEmbeddedInteger());
    
    frame->Push((VMObject*) result);
}


void  _BigInteger::Equal(VMObject* object, VMFrame* frame) {
    VMObject* rightObj  = frame->Pop();
    VMBigInteger* right = NULL;
    VMBigInteger* left  = (VMBigInteger*)frame->Pop();
    
    CHECK_BIGINT(rightObj, right);   
    
    // Do operation:
    if(left->GetEmbeddedInteger() == right->GetEmbeddedInteger())
        frame->Push(true_object);
    else
        frame->Push(false_object);
}


void  _BigInteger::Lowerthan(VMObject* object, VMFrame* frame) {
    VMObject* rightObj  = frame->Pop();
    VMBigInteger* right = NULL;
    VMBigInteger* left  = (VMBigInteger*)frame->Pop();
    
    CHECK_BIGINT(rightObj, right);   
    
    // Do operation:
    if(left->GetEmbeddedInteger() < right->GetEmbeddedInteger())
        frame->Push(true_object);
    else
        frame->Push(false_object);
     
}


void  _BigInteger::AsString(VMObject* object, VMFrame* frame) {
    VMBigInteger* self = (VMBigInteger*)frame->Pop();
    // temporary storage for the number string
    // use c99 snprintf-goodie
    int64_t bigint = self->GetEmbeddedInteger();
    ostringstream Str;
    Str << bigint;
    frame->Push((VMObject*)_UNIVERSE->new_string(pString(Str.str())));
    
}


void  _BigInteger::Sqrt(VMObject* object, VMFrame* frame) {
    VMBigInteger* self = (VMBigInteger*)frame->Pop();
    int64_t i = self->GetEmbeddedInteger();
    frame->Push((VMObject*)_UNIVERSE->new_double(sqrt((double)i)));
}
