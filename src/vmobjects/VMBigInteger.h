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

    void SetEmbeddedInteger(int64_t);
    int64_t GetEmbeddedInteger();

private:
    int64_t embeddedInteger;
};

#endif
