#include "VMBigInteger.h"


VMBigInteger::VMBigInteger() : VMObject() {
    this->embeddedInteger = 0;
}


VMBigInteger::VMBigInteger(int64_t val) : VMObject() {
    this->embeddedInteger = val;
}


int64_t VMBigInteger::GetEmbeddedInteger() {
    return this->embeddedInteger;
}


void VMBigInteger::SetEmbeddedInteger(int64_t val) {
    this->embeddedInteger = val;
}


//VMBigInteger::~VMBigInteger()
//{
//}
