#pragma once

#ifndef VMBIGINTEGER_H_
#define VMBIGINTEGER_H_
#include "../misc/defs.h"
#include "VMObject.h"


class VMBigInteger : public VMObject {
public:
    VMBigInteger();
    VMBigInteger(int64_t);
    //virtual ~VMBigInteger();

    inline void SetEmbeddedInteger(int64_t);
    inline int64_t GetEmbeddedInteger() const;

private:
    int64_t embeddedInteger;

    static const int VMBigIntegerNumberOfFields;
};

int64_t VMBigInteger::GetEmbeddedInteger() const {
    return this->embeddedInteger;
}


void VMBigInteger::SetEmbeddedInteger(int64_t val) {
    this->embeddedInteger = val;
}
#endif
