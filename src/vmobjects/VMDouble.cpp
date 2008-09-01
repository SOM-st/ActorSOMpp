#include "VMDouble.h"

VMDouble::VMDouble() : VMObject()
{
    this->embeddedDouble = 0.0f;
}

VMDouble::VMDouble(double val) : VMObject()
{
    this->embeddedDouble = val;
}

void VMDouble::SetEmbeddedDouble(double val)
{
    this->embeddedDouble = val;
}

double VMDouble::GetEmbeddedDouble()
{
    return this->embeddedDouble;
}

VMDouble::~VMDouble()
{
}
