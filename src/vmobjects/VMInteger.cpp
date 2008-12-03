#include "VMInteger.h"


VMInteger::VMInteger() : VMObject() {
    embeddedInteger = 0;
}


VMInteger::VMInteger(int32_t val) : VMObject() {
    embeddedInteger = val;
}




//VMInteger::~VMInteger()
//{
//}
