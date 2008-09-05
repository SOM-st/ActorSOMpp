#pragma once

#ifndef VMINTEGER_H_
#define VMINTEGER_H_

#include "VMObject.h"
#include "../misc/defs.h"

class VMInteger : public VMObject
{
public:
    VMInteger();
    VMInteger(int32_t);
    virtual ~VMInteger();
    void SetEmbeddedInteger(int32_t);
    int32_t GetEmbeddedInteger();
    
    VMInteger operator++(int)
    {
        ++embeddedInteger;
        return *this;
    };
    VMInteger operator++()
    {
        ++embeddedInteger;
        return *this;
    };

    VMInteger operator--(int)
    {
        --embeddedInteger;
        return *this;
    };
    VMInteger operator--()
    {
        --embeddedInteger;
        return *this;
    };

    VMInteger operator+(VMInteger op2)
    {
        embeddedInteger += op2.GetEmbeddedInteger();
        return *this;
    };

    VMInteger operator-(VMInteger op2)
    {
        embeddedInteger -= op2.GetEmbeddedInteger();
        return *this;
    };

private:
    int32_t embeddedInteger;
};

#endif