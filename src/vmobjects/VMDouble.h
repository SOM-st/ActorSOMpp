#pragma once

#ifndef VMDOUBLE_H_
#define VMDOUBLE_H_

#include "VMObject.h"


class VMDouble : public VMObject {
public:
    VMDouble();
    VMDouble(double);
    //virtual ~VMDouble();
    inline void    SetEmbeddedDouble(double);
    inline double  GetEmbeddedDouble() const;
private:
    double embeddedDouble;
};

void VMDouble::SetEmbeddedDouble(double val) {
    this->embeddedDouble = val;
}


double VMDouble::GetEmbeddedDouble() const {
    return this->embeddedDouble;
}

#endif
