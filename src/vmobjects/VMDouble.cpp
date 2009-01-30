#include "VMDouble.h"

const int VMDouble::VMDoubleNumberOfFields = 0; 

VMDouble::VMDouble() : VMObject(VMDoubleNumberOfFields) {
    this->embeddedDouble = 0.0f;
}


VMDouble::VMDouble(double val) : VMObject(VMDoubleNumberOfFields) {
    this->embeddedDouble = val;
}



//VMDouble::~VMDouble()
//{
//}
