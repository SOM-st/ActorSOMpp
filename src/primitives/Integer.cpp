/*
 * $Id: Integer.c 249 2008-04-28 08:17:23Z michael.haupt $
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

#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <limits.h>
#include <sstream>

#include "../vmobjects/VMObject.h"
#include "../vmobjects/VMFrame.h"
#include "../vmobjects/VMDouble.h"
#include "../vmobjects/VMInteger.h"
#include "../vmobjects/VMBigInteger.h"

#include "../vm/Universe.h"
#include "Core.h"
#include "Integer.h"
#include "Routine.h"
_Integer* Integer;

/*
 * This macro performs a coercion check to BigInteger and Double. Depending on
 * the right-hand operand, an Integer operation will have to be resent as a
 * BigInteger or Double operation (those types impose themselves on the result
 * of an Integer operation).
 */
#define CHECK_COERCION(obj,receiver,op) { \
    if(dynamic_cast<VMBigInteger*>(obj) != NULL) { \
        resendAsBigInteger( \
            object, (op), (receiver), (VMBigInteger*)(obj)); \
        return; \
    } else if(dynamic_cast<VMDouble*>(obj) != NULL) { \
        resendAsDouble( \
            object, (op), (receiver), (VMDouble*)(obj)); \
        return; \
    } \
}
    

//
// private functions for Integer
//


void _Integer::pushResult(VMObject* /*object*/, VMFrame* frame, 
                              int64_t result) {
    int32_t i32min = INT32_MIN;
    // Check with integer bounds and push:
    if(result > INT32_MAX || result < i32min)
        frame->Push((VMObject*)universe->NewBigInteger(result));
    else
        frame->Push((VMObject*)universe->NewInteger((int32_t)result));
}


void _Integer::resendAsBigInteger(VMObject* /*object*/, 
                                  const char* op,
                                  VMInteger* left, VMBigInteger* right) {
    // Construct left value as BigInteger:
    VMBigInteger* leftBigInteger = 
        universe->NewBigInteger((int64_t)left->GetEmbeddedInteger());
    
    // Resend message:
    VMObject* operands[] = { (VMObject*)right };
    
    pString ops = pString(op);
    leftBigInteger->Send(ops, operands, 1);
    // no reference
    //SEND(op, free);
}


void _Integer::resendAsDouble(VMObject* /*object*/, const char* op,
    VMInteger* left, VMDouble* right
) {
    VMDouble* leftDouble =
        universe->NewDouble((double)left->GetEmbeddedInteger());
    VMObject* operands[] = { (VMObject*)right };
    pString ops = pString(op);
    leftDouble->Send(ops, operands, 1);
    //SEND(op, free);
}


_Integer::_Integer() {
    srand((unsigned) time(NULL)) ;
}


//
// arithmetic operations
//


void  _Integer::Plus(VMObject* object, VMFrame* frame) {
    VMObject* rightObj = frame->Pop();
    VMInteger* left = (VMInteger*)frame->Pop();
    
    CHECK_COERCION(rightObj, left, "+");

    // Do operation:
    VMInteger* right = (VMInteger*)rightObj;
    
    int64_t result = (int64_t)left->GetEmbeddedInteger() + 
        (int64_t)right->GetEmbeddedInteger();
    pushResult(object, frame, result);
}


void  _Integer::Minus(VMObject* object, VMFrame* frame) {
    VMObject* rightObj = frame->Pop();
    VMInteger* left = (VMInteger*)frame->Pop();
    
    CHECK_COERCION(rightObj, left, "-");

    // Do operation:
    VMInteger* right = (VMInteger*)rightObj;
    
    int64_t result = (int64_t)left->GetEmbeddedInteger() - 
        (int64_t)right->GetEmbeddedInteger();
    pushResult(object, frame, result);
}


void  _Integer::Star(VMObject* object, VMFrame* frame) {
    VMObject* rightObj = frame->Pop();
    VMInteger* left = (VMInteger*)frame->Pop();
    
    CHECK_COERCION(rightObj, left, "*");

    // Do operation:
    VMInteger* right = (VMInteger*)rightObj;
    
    int64_t result = (int64_t)left->GetEmbeddedInteger() * 
        (int64_t)right->GetEmbeddedInteger();
    pushResult(object, frame, result); 
}


void  _Integer::Slashslash(VMObject* object, VMFrame* frame) {
    VMObject* rightObj = frame->Pop();
    VMInteger* left = (VMInteger*)frame->Pop();
    
    CHECK_COERCION(rightObj, left, "/");

    // Do operation:
    VMInteger* right = (VMInteger*)rightObj;
    
    double result = (double)left->GetEmbeddedInteger() /
        (double)right->GetEmbeddedInteger();
    frame->Push(universe->NewDouble(result));
}


void  _Integer::Slash(VMObject* object, VMFrame* frame) {
    VMObject* rightObj = frame->Pop();
    VMInteger* left = (VMInteger*)frame->Pop();
    
    CHECK_COERCION(rightObj, left, "/");

    // Do operation:
    VMInteger* right = (VMInteger*)rightObj;
    
    int64_t result = (int64_t)left->GetEmbeddedInteger() / 
        (int64_t)right->GetEmbeddedInteger();
    pushResult(object, frame, result); 
}


void  _Integer::Percent(VMObject* object, VMFrame* frame) {
    VMObject* rightObj = frame->Pop();
    VMInteger* left = (VMInteger*)frame->Pop();
    
    CHECK_COERCION(rightObj, left, "%");

    // Do operation:
    VMInteger* right = (VMInteger*)rightObj;

    int64_t result = (int64_t)left->GetEmbeddedInteger() %
        (int64_t)right->GetEmbeddedInteger();
    pushResult(object, frame, result); 
}


void  _Integer::And(VMObject* object, VMFrame* frame) {
    VMObject* rightObj = frame->Pop();
    VMInteger* left = (VMInteger*)frame->Pop();
    
    CHECK_COERCION(rightObj, left, "&");

    // Do operation:
    VMInteger* right = (VMInteger*)rightObj;
    
    int64_t result = (int64_t)left->GetEmbeddedInteger() & 
        (int64_t)right->GetEmbeddedInteger();
    pushResult(object, frame, result); 
}   


void  _Integer::Equal(VMObject* object, VMFrame* frame) {
    VMObject* rightObj = frame->Pop();
    VMInteger* left = (VMInteger*)frame->Pop();
    
    CHECK_COERCION(rightObj, left, "=");

    if(dynamic_cast<VMInteger*>(rightObj) != NULL) {
        // Second operand was Integer:
        VMInteger* right = (VMInteger*)rightObj;
        
        if(left->GetEmbeddedInteger()
            == right->GetEmbeddedInteger())
            frame->Push(Globals::TrueObject());
        else
            frame->Push(Globals::FalseObject());
    } else if(dynamic_cast<VMDouble*>(rightObj) != NULL) {
        // Second operand was Double:
        VMDouble* right = (VMDouble*)rightObj;
        
        if((double)left->GetEmbeddedInteger()
            == right->GetEmbeddedDouble())
            frame->Push(Globals::TrueObject());
        else
            frame->Push(Globals::FalseObject());
    }
    else
        frame->Push(Globals::FalseObject());
}


void  _Integer::Lowerthan(VMObject* object, VMFrame* frame) {
    VMObject* rightObj = frame->Pop();
    VMInteger* left = (VMInteger*)frame->Pop();
    
    CHECK_COERCION(rightObj, left, "<");

    VMInteger* right = (VMInteger*)rightObj;
    
    if(left->GetEmbeddedInteger() < right->GetEmbeddedInteger())
        frame->Push(Globals::TrueObject());
    else
        frame->Push(Globals::FalseObject());
}


void  _Integer::AsString(VMObject* /*object*/, VMFrame* frame) {
    VMInteger* self = (VMInteger*)frame->Pop();
    // temporary storage for the number string
    // use c99 snprintf-goodie
    int32_t integer = self->GetEmbeddedInteger();
    ostringstream Str;
    Str << integer;
    frame->Push( (VMObject*)universe->NewString( pString(Str.str()) ) );   
}


void  _Integer::Sqrt(VMObject* /*object*/, VMFrame* frame) {
    VMInteger* self = (VMInteger*)frame->Pop();
    double result = sqrt((double)self->GetEmbeddedInteger());
    frame->Push((VMObject*)universe->NewDouble(result));
}


void  _Integer::AtRandom(VMObject* /*object*/, VMFrame* frame) {
    VMInteger* self = (VMInteger*)frame->Pop();
    int32_t result = (self->GetEmbeddedInteger() * rand())%INT32_MAX;
    frame->Push((VMObject*) universe->NewInteger(result));
}

PrimitiveRoutine* _Integer::GetRoutine( const pString& routineName )
{
    PrimitiveRoutine* result;
    if (routineName == pString("Plus"))
        result = new (heap) Routine<_Integer>(Integer, &_Integer::Plus);
    else if (routineName == pString("Minus"))
        result = new (heap) Routine<_Integer>(Integer, &_Integer::Minus);
    else if (routineName == pString("Star"))
        result = new (heap) Routine<_Integer>(Integer, &_Integer::Star);
    else if (routineName == pString("Slash"))
        result = new (heap) Routine<_Integer>(Integer, &_Integer::Slash);
    else if (routineName == pString("Slashslash"))
        result = new (heap) Routine<_Integer>(Integer, &_Integer::Slashslash);
    else if (routineName == pString("Percent"))
        result = new (heap) Routine<_Integer>(Integer, &_Integer::Percent);
    else if (routineName == pString("And"))
        result = new (heap) Routine<_Integer>(Integer, &_Integer::And);
    else if (routineName == pString("Equal"))
        result = new (heap) Routine<_Integer>(Integer, &_Integer::Equal);
    else if (routineName == pString("Lowerthan"))
        result = new (heap) Routine<_Integer>(Integer, &_Integer::Lowerthan);
    else if (routineName == pString("AsString"))
        result = new (heap) Routine<_Integer>(Integer, &_Integer::AsString);
    else if (routineName == pString("Sqrt"))
        result = new (heap) Routine<_Integer>(Integer, &_Integer::Sqrt);
    else if (routineName == pString("AtRandom"))
        result = new (heap) Routine<_Integer>(Integer, &_Integer::AtRandom);
    else {
        cout << "method " << routineName << "not found in class Integer" << endl;
        return NULL;
    }
    return result;
}

