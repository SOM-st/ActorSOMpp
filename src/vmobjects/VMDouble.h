#pragma once

#ifndef VMDOUBLE_H_
#define VMDOUBLE_H_

#include "VMObject.h"


class VMDouble : public VMObject {
public:
    VMDouble();
    VMDouble(double);
    //virtual ~VMDouble();
    void    SetEmbeddedDouble(double);
    double  GetEmbeddedDouble();
private:
    double embeddedDouble;
};

#endif
