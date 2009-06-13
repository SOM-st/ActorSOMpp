/*
 *  VMPointer.cpp
 *  SOM
 *
 *  Created by Daniel Hefenbrock on 5/21/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "VMPointer.h"

#include "../vm/Universe.h"

#include "VMInteger.h"

// twos complement for with 31 bits
#define MAX_IINT    1073741823
#define MIN_IINT   -1073741824 

void VMPointerSetValueOnProxy(int32_t value) {
    integerProxy->SetEmbeddedInteger(value);
};

template<class T>
void VMPointer<T>::SetEmbeddedInteger(int32_t val) {
    if (index.index.is_iint) {
        index.iint.value = val;
        if (val > MAX_IINT || val < MIN_IINT) {
            index.index = _UNIVERSE->NewIntegerVMObject(val).index.index;
        }
    } else {
        ((VMInteger*) _OBJECT_TABLE[index.index])->SetEmbeddedInteger(val);
    }
}

template<class T>
int32_t VMPointer<T>::GetEmbeddedInteger() const {
    if (index.index.is_iint) {
        return index.iint.value;
    } else {
        return ((VMInteger*) _OBJECT_TABLE[index.index])->GetEmbeddedInteger();
    } 
}


template void VMPointer<VMInteger>::SetEmbeddedInteger(int32_t val);
template int32_t VMPointer<VMInteger>::GetEmbeddedInteger() const;
template int32_t VMPointer<VMObject>::GetEmbeddedInteger() const;
//template void VMPointer<VMObject>::SetValueOnProxy() const;
//template void VMPointer<VMInteger>::SetValueOnProxy() const;