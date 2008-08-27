#include "VMMethod.h"

VMMethod::VMMethod(int bc_count) : VMObject(), VMInvokable()
{
	objectSize = sizeof(VMMethod);
    bc_length = bc_count;
    bc = (uint8_t*)&bc + sizeof(uint8_t*);
}

void VMMethod::invoke(VMFrame* frame)
{
    //create new frame, add this, copy args from passed frame
}

void VMMethod::set_holder_all(VMClass* hld)
{
}

VMObject* VMMethod::get_constant(int indx)
{
    //VMArray* 
    return (VMObject*)bc[indx];
}

uint8_t VMMethod::get_bytecode(int indx)
{
    return bc[indx];
}

void VMMethod::set_bytecode(int indx, uint8_t val)
{
    bc[indx] = val;
}

VMMethod::~VMMethod() {}

