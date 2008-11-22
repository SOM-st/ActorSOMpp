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

#include "../vm/universe.h"

#include "../primitivesCore/Routine.h"
 
#include "BigInteger.h"


#define CHECK_BIGINT(object, result) { \
    /* Check second parameter type: */ \
    VMInteger* ptr;\
    if((ptr = dynamic_cast<VMInteger*>(object)) != NULL) { \
        /* Second operand was Integer*/ \
        int32_t i = ptr->GetEmbeddedInteger(); \
        (result) = _UNIVERSE->NewBigInteger((int64_t)i); \
    } else \
        (result) = (VMBigInteger*)(object); \
}

#define PUSH_INT_OR_BIGINT(result) { \
    if(result > INT32_MAX ||result < INT32_MIN) \
        frame->Push((VMObject*)_UNIVERSE->NewBigInteger((result))); \
    else \
        frame->Push((VMObject*)_UNIVERSE->NewInteger((int32_t)(result))); \
}
//^^DIFFERENT THAN CSOM! Does the CSOM version work at all????????


_BigInteger::_BigInteger( ) : Primitive(){
    this->SetRoutine("plus", static_cast<PrimitiveRoutine*>(
        new (_HEAP) Routine<_BigInteger>(this, &_BigInteger::Plus)));

    this->SetRoutine("minus", static_cast<PrimitiveRoutine*>(
        new (_HEAP) Routine<_BigInteger>(this, &_BigInteger::Minus)));

    this->SetRoutine("star", static_cast<PrimitiveRoutine*>(
        new (_HEAP) Routine<_BigInteger>(this, &_BigInteger::Star)));

    this->SetRoutine("slash", static_cast<PrimitiveRoutine*>( 
        new (_HEAP) Routine<_BigInteger>(this, &_BigInteger::Slash)));

    this->SetRoutine("percent", static_cast<PrimitiveRoutine*>(
        new (_HEAP) Routine<_BigInteger>(this, &_BigInteger::Percent)));

    this->SetRoutine("and", static_cast<PrimitiveRoutine*>(
        new (_HEAP) Routine<_BigInteger>(this, &_BigInteger::And)));

    this->SetRoutine("equal", static_cast<PrimitiveRoutine*>(
        new (_HEAP) Routine<_BigInteger>(this, &_BigInteger::Equal)));

    this->SetRoutine("lowerthan", static_cast<PrimitiveRoutine*>(
        new (_HEAP) Routine<_BigInteger>(this, &_BigInteger::Lowerthan)));

    this->SetRoutine("asString", static_cast<PrimitiveRoutine*>(
        new (_HEAP) Routine<_BigInteger>(this, &_BigInteger::AsString)));

    this->SetRoutine("sqrt", static_cast<PrimitiveRoutine*>(
        new (_HEAP) Routine<_BigInteger>(this, &_BigInteger::Sqrt)));

}


void  _BigInteger::Plus(VMObject* /*object*/, VMFrame* frame) {
    VMObject* rightObj  = frame->Pop();
    VMBigInteger* right = NULL;
    VMBigInteger* left  = (VMBigInteger*)frame->Pop();
    
    CHECK_BIGINT(rightObj, right);
    
    // Do operation and perform conversion to Integer if required
    int64_t result = left->GetEmbeddedInteger()
                    + right->GetEmbeddedInteger();
    PUSH_INT_OR_BIGINT(result);
}


void  _BigInteger::Minus(VMObject* /*object*/, VMFrame* frame) {
    VMObject* rightObj  = frame->Pop();
    VMBigInteger* right = NULL;
    VMBigInteger* left  = (VMBigInteger*)frame->Pop();
    
    CHECK_BIGINT(rightObj, right);   
    
    // Do operation and perform conversion to Integer if required
    int64_t result =  left->GetEmbeddedInteger()
                    - right->GetEmbeddedInteger();
    PUSH_INT_OR_BIGINT(result);
}


void  _BigInteger::Star(VMObject* /*object*/, VMFrame* frame) {
   VMObject* rightObj  = frame->Pop();
    VMBigInteger* right = NULL;
    VMBigInteger* left  = (VMBigInteger*)frame->Pop();
    
    CHECK_BIGINT(rightObj, right);   
    
    // Do operation and perform conversion to Integer if required
    int64_t result =  left->GetEmbeddedInteger()
                    * right->GetEmbeddedInteger();
    PUSH_INT_OR_BIGINT(result);
}


void  _BigInteger::Slash(VMObject* /*object*/, VMFrame* frame) {
    VMObject* rightObj  = frame->Pop();
    VMBigInteger* right = NULL;
    VMBigInteger* left  = (VMBigInteger*)frame->Pop();
    
    CHECK_BIGINT(rightObj, right);   
    
    // Do operation and perform conversion to Integer if required
    int64_t result =  left->GetEmbeddedInteger()
                    / right->GetEmbeddedInteger();
    PUSH_INT_OR_BIGINT(result);
}


void  _BigInteger::Percent(VMObject* /*object*/, VMFrame* frame) {
    VMObject* rightObj  = frame->Pop();
    VMBigInteger* right = NULL;
    VMBigInteger* left  = (VMBigInteger*)frame->Pop();
    
    CHECK_BIGINT(rightObj, right);   
    
    // Do operation and perform conversion to Integer if required
    VMBigInteger* result = _UNIVERSE->NewBigInteger(  left->GetEmbeddedInteger()
                                                    % right->GetEmbeddedInteger());
    
    frame->Push((VMObject*) result);
}


void  _BigInteger::And(VMObject* /*object*/, VMFrame* frame) {
    VMObject* rightObj  = frame->Pop();
    VMBigInteger* right = NULL;
    VMBigInteger* left  = (VMBigInteger*)frame->Pop();
    
    CHECK_BIGINT(rightObj, right);   
    
    // Do operation and perform conversion to Integer if required
    VMBigInteger* result = _UNIVERSE->NewBigInteger(  left->GetEmbeddedInteger()
                                                    & right->GetEmbeddedInteger());
    
    frame->Push((VMObject*) result);
}


void  _BigInteger::Equal(VMObject* /*object*/, VMFrame* frame) {
    VMObject* rightObj  = frame->Pop();
    VMBigInteger* right = NULL;
    VMBigInteger* left  = (VMBigInteger*)frame->Pop();
    
    CHECK_BIGINT(rightObj, right);   
    
    // Do operation:
    if(left->GetEmbeddedInteger() == right->GetEmbeddedInteger())
        frame->Push(Globals::TrueObject());//Globals::TrueObject());
    else
        frame->Push(Globals::FalseObject());
}


void  _BigInteger::Lowerthan(VMObject* /*object*/, VMFrame* frame) {
    VMObject* rightObj  = frame->Pop();
    VMBigInteger* right = NULL;
    VMBigInteger* left  = (VMBigInteger*)frame->Pop();
    
    CHECK_BIGINT(rightObj, right);   
    
    // Do operation:
    if(left->GetEmbeddedInteger() < right->GetEmbeddedInteger())
        frame->Push(Globals::TrueObject());
    else
        frame->Push(Globals::FalseObject());
     
}


void  _BigInteger::AsString(VMObject* /*object*/, VMFrame* frame) {
    VMBigInteger* self = (VMBigInteger*)frame->Pop();
    // temporary storage for the number string
    // use c99 snprintf-goodie
    int64_t bigint = self->GetEmbeddedInteger();
    ostringstream Str;
    Str << bigint;
    frame->Push((VMObject*)_UNIVERSE->NewString(pString(Str.str())));
    
}


void  _BigInteger::Sqrt(VMObject* /*object*/, VMFrame* frame) {
    VMBigInteger* self = (VMBigInteger*)frame->Pop();
    int64_t i = self->GetEmbeddedInteger();
    frame->Push((VMObject*)_UNIVERSE->NewDouble(sqrt((double)i)));
}


