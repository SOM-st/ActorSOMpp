#include "VMMethod.h"
#include "../vm/Universe.h"
#include "VMFrame.h"
#include "../compiler/MethodGenerationContext.h"

VMMethod::VMMethod(int bc_count, int number_of_constants) : VMArray((bc_count/sizeof(VMObject*)) + number_of_constants ), VMInvokable()
{
	objectSize = sizeof(VMMethod) + bc_count + number_of_constants*sizeof(VMObject*);
    bc_length = _UNIVERSE->new_integer( bc_count );
    number_of_locals = _UNIVERSE->new_integer(0);
    maximum_number_of_stack_elements = _UNIVERSE->new_integer(0);
    number_of_arguments = _UNIVERSE->new_integer(0);
    bc = (uint8_t*)&bc + sizeof(uint8_t*);
    this->ResetEntriesPointer();
    //theEntries = (VMObject**)&theEntries + this->GetOffset();
}

int       VMMethod::get_number_of_locals() 
{
    return number_of_locals->GetEmbeddedInteger(); 
}

void      VMMethod::set_number_of_locals(int nol) 
{
    number_of_locals->SetEmbeddedInteger(nol); 
}

int       VMMethod::get_maximum_number_of_stack_elements()
{
    return maximum_number_of_stack_elements->GetEmbeddedInteger(); 
}

void      VMMethod::set_maximum_number_of_stack_elements(int stel) 
{
    maximum_number_of_stack_elements->SetEmbeddedInteger(stel); 
}

int       VMMethod::get_number_of_arguments() 
{
    return number_of_arguments->GetEmbeddedInteger(); 
}

void      VMMethod::set_number_of_arguments(int noa) 
{
    number_of_arguments->SetEmbeddedInteger(noa); 
}

int       VMMethod::get_number_of_bytecodes() 
{
    return bc_length->GetEmbeddedInteger();
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
   // for(int i = this->
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

