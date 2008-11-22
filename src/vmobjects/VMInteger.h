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
    void            SetEmbeddedInteger(int32_t);
    int32_t         GetEmbeddedInteger();
private:
    int32_t embeddedInteger;
};

#endif
