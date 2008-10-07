#include "VMMethod.h"
#include "../vm/Universe.h"
#include "VMFrame.h"
#include "../compiler/MethodGenerationContext.h"
#include "VMClass.h"
#include "VMSymbol.h"
#include "VMArray.h"
#include "VMObject.h"
#include "VMInteger.h"

#define _BC ((uint8_t*)&fields[this->GetNumberOfFields() + this->GetNumberOfIndexableFields()])

#define theEntries(i) fields[this->GetNumberOfFields()+i]

VMMethod::VMMethod(int bc_count, int number_of_constants, int nof) :  VMInvokable(nof + 5)//VMArray((bc_count/sizeof(VMObject*)) + number_of_constants ),
{
    objectSize += bc_count + number_of_constants*sizeof(VMObject*);
    bc_length = _UNIVERSE->new_integer( bc_count );
    number_of_locals = _UNIVERSE->new_integer(0);
    maximum_number_of_stack_elements = _UNIVERSE->new_integer(0);
    number_of_arguments = _UNIVERSE->new_integer(0);
    size = _UNIVERSE->new_integer(number_of_constants);
}

void VMMethod::MarkReferences()
{
    VMInvokable::MarkReferences();
    number_of_locals->MarkReferences();
    maximum_number_of_stack_elements->MarkReferences();
    number_of_arguments->MarkReferences();
    size->MarkReferences();
    bc_length->MarkReferences();
}

int VMMethod::get_number_of_locals() 
{
    return number_of_locals->GetEmbeddedInteger(); 
}

void VMMethod::set_number_of_locals(int nol) 
{
    number_of_locals->SetEmbeddedInteger(nol); 
}

int VMMethod::get_maximum_number_of_stack_elements()
{
    return maximum_number_of_stack_elements->GetEmbeddedInteger(); 
}

void VMMethod::set_maximum_number_of_stack_elements(int stel) 
{
    maximum_number_of_stack_elements->SetEmbeddedInteger(stel); 
}

int VMMethod::get_number_of_arguments() 
{
    return number_of_arguments->GetEmbeddedInteger(); 
}

void VMMethod::set_number_of_arguments(int noa) 
{
    number_of_arguments->SetEmbeddedInteger(noa); 
}

int VMMethod::get_number_of_bytecodes() 
{
    return bc_length->GetEmbeddedInteger();
}

void VMMethod::invoke(VMFrame* frame)
{
    VMFrame* frm = _UNIVERSE->GetInterpreter()->PushNewFrame(this);
    frm->CopyArgumentsFrom(frame);
}

void VMMethod::set_holder_all(VMClass* hld)
{
    for (int i = 0; i < this->GetNumberOfIndexableFields(); ++i)
    {
        VMObject* o = GetIndexableField(i);
        if (dynamic_cast<VMInvokable*>(o) != NULL) 
        {
            ((VMInvokable*)o)->set_holder(hld);
        }
    }
}

VMObject* VMMethod::get_constant(int indx)
{
    uint8_t bc = _BC[indx+1];
    if (bc >= this->GetNumberOfIndexableFields()) {
        cout << "Error: Constant index out of range" << endl;
        return NULL;
    }
    return this->GetIndexableField(bc);
}

uint8_t VMMethod::get_bytecode(int indx)
{
    return _BC[indx];
}


void VMMethod::set_bytecode(int indx, uint8_t val)
{
    _BC[indx] = val;
}

//VMArray Methods
VMArray* VMMethod::CopyAndExtendWith(VMObject* item)
{
    size_t fields = this->size->GetEmbeddedInteger();
	VMArray* result = _UNIVERSE->new_array(fields+1);
    this->CopyIndexableFieldsTo(result);
	result->SetIndexableField(fields, item);
	return result;
}

VMObject* VMMethod::GetIndexableField(int idx)
{
    if (idx > size->GetEmbeddedInteger()-1 || idx < 0)  {
        cout << "Array index out of bounds: Accessing " << idx << ", but only " << size->GetEmbeddedInteger()-1;
        cout << " entries are available\n";
        _UNIVERSE->error_exit("Array index out of bounds exception");
        //throw std::bad_exception();
    }
    return theEntries(idx);
}

void VMMethod::CopyIndexableFieldsTo(VMArray* to)
{
	for (int i = 0; i < this->GetNumberOfIndexableFields(); ++i)
	{
        to->SetIndexableField(i, this->GetIndexableField(i));
	}
	
}

int VMMethod::GetNumberOfIndexableFields()
{
    return size->GetEmbeddedInteger();
}


void VMMethod::SetIndexableField(int idx, VMObject* item)
{
	if (idx > size->GetEmbeddedInteger()-1 || idx < 0) {
        cout << "Array index out of bounds: Accessing " << idx << ", but there is only space for " << size->GetEmbeddedInteger();
        cout << " entries available\n";
        _UNIVERSE->error_exit("Array index out of bounds exception");
        //throw std::bad_exception();
    }
   	theEntries(idx) = item;
}

//VMMethod::~VMMethod() {}

