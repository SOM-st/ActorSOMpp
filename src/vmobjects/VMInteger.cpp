#include "VMInteger.h"

VMInteger::VMInteger() : VMObject()
{
    embeddedInteger = 0;
}

VMInteger::VMInteger(int32_t val) : VMObject()
{
    embeddedInteger = val;
}

void VMInteger::SetEmbeddedInteger(int32_t val)
{
    this->embeddedInteger = val;
}

int32_t VMInteger::GetEmbeddedInteger()
{
    return this->embeddedInteger;
}

VMInteger::~VMInteger()
{
}
