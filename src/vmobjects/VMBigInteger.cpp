#include "VMBigInteger.h"

const int VMBigInteger::VMBigIntegerNumberOfFields = 0; 

VMBigInteger::VMBigInteger() : VMObject(VMBigIntegerNumberOfFields) {
    this->embeddedInteger = 0;
}


VMBigInteger::VMBigInteger(int64_t val) : VMObject(VMBigIntegerNumberOfFields) {
    this->embeddedInteger = val;
}



//VMBigInteger::~VMBigInteger()
//{
//}
