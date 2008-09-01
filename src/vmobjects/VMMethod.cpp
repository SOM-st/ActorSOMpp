#include "VMMethod.h"
#include "../vm/Universe.h"
#include "VMFrame.h"
#include "../compiler/MethodGenerationContext.h"


VMMethod::VMMethod(int bc_count, int number_of_constants) : VMArray(bc_count+number_of_constants), VMInvokable()
{
	objectSize = sizeof(VMMethod);
    bc_length = bc_count;
    bc = (uint8_t*)&bc + sizeof(uint8_t*);
}

//VMMethod::VMMethod(MethodGenerationContext* mgenc) : VMArray(mgenc->), VMInvokable()
//{
//}

size_t VMMethod::GetOffset()
{
    return VMArray::GetOffset() + sizeof(int)*4 + sizeof(uint8_t*);
}

void VMMethod::invoke(VMFrame* frame)
{
    //VMFrame* frm = new (_HEAP)VMFrame(this);
    //frm->CopyArgumentsFrom(frame);
}

void VMMethod::set_holder_all(VMClass* hld)
{
    //go through indexable_fields
    //if (typeid(indexable_field[i]).name() == "class VMInvokable *") indexable_field[i]->setHolder(hld);
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

