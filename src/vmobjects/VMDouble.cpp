#include "VMDouble.h"


VMDouble::VMDouble() : VMObject() {
    this->embeddedDouble = 0.0f;
}


VMDouble::VMDouble(double val) : VMObject() {
    this->embeddedDouble = val;
}



//VMDouble::~VMDouble()
//{
//}
