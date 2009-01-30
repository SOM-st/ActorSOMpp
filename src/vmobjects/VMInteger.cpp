#include "VMInteger.h"

const int VMInteger::VMIntegerNumberOfFields = 0; 

VMInteger::VMInteger() : VMObject(VMIntegerNumberOfFields) {
    embeddedInteger = 0;
}


VMInteger::VMInteger(int32_t val) : VMObject(VMIntegerNumberOfFields) {
    embeddedInteger = val;
}




//VMInteger::~VMInteger()
//{
//}
