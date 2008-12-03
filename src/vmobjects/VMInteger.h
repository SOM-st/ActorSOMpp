#pragma once

#ifndef VMINTEGER_H_
#define VMINTEGER_H_

#include "VMObject.h"
#include "../misc/defs.h"


class VMInteger : public VMObject {
public:
    VMInteger();
    VMInteger(int32_t);
    //virtual ~VMInteger();
    inline void            SetEmbeddedInteger(int32_t);
    inline int32_t         GetEmbeddedInteger() const;
private:
    int32_t embeddedInteger;
};

void VMInteger::SetEmbeddedInteger(int32_t val) {
    this->embeddedInteger = val;
}


int32_t VMInteger::GetEmbeddedInteger() const {
    return this->embeddedInteger;
}

#endif
