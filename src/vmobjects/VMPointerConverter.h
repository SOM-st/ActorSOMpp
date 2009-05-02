#pragma once

#ifndef VMPOINTERCONVERTER_H_
#define VMPOINTERCONVERTER_H_

#include "VMPointer.h"

//This template fucntion converts smart pointer of one type into
//smart pointers of a different type using type checking.
template<class TO, class FROM>
static VMPointer<TO> DynamicConvert(VMPointer<FROM> pu) {
    TO* ptr = dynamic_cast<TO*>(pu.GetPointer());
    //if (ptr == NULL) ptr = (T*)Globals::NilObject().GetPointer();
    return VMPointer<TO>(ptr);
};


#endif

