#pragma once

#ifndef VMPOINTERCONVERTER_H_
#define VMPOINTERCONVERTER_H_

#include "VMPointer.h"

//This template fucntion converts smart pointer of one type into
//smart pointers of a different type using type checking.
template<class TO, class FROM>
static VMPointer<TO> DynamicConvert(VMPointer<FROM> pu) {
    // This also performs type checking.
    return VMPointer<TO>(pu, true);
};


#endif

