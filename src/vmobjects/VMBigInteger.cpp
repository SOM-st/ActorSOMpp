#include "VMBigInteger.h"


VMBigInteger::VMBigInteger() : VMObject() {
    this->embeddedInteger = 0;
}


VMBigInteger::VMBigInteger(int64_t val) : VMObject() {
    this->embeddedInteger = val;
}



//VMBigInteger::~VMBigInteger()
//{
//}
