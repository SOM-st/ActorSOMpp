#include "VMMethod.h"
#include "../vm/Universe.h"
#include "VMFrame.h"
#include "../compiler/MethodGenerationContext.h"
#include "VMClass.h"
#include "VMSymbol.h"

#define _BC ((uint8_t*)&fields[this->GetNumberOfFields() + 1 + this->GetOffset()])

VMMethod::VMMethod(int bc_count, int number_of_constants, int nof) :  VMInvokable(nof + 5)//VMArray((bc_count/sizeof(VMObject*)) + number_of_constants ),
{
    //this->SetNumberOfFields(this->GetNumberOfFields() + 4);
    objectSize += bc_count + number_of_constants*sizeof(VMObject*);
    bc_length = _UNIVERSE->new_integer( bc_count );
    number_of_literals = _UNIVERSE->new_integer(0);
    number_of_locals = _UNIVERSE->new_integer(0);
    maximum_number_of_stack_elements = _UNIVERSE->new_integer(0);
    number_of_arguments = _UNIVERSE->new_integer(0);
    //uint8_t* bc = _BC;
    //this->ResetEntriesPointer();
    //theEntries = (VMObject**)&theEntries + this->GetOffset();
}

void VMMethod::MarkReferences()
{
    //VMArray::MarkReferences();
    VMInvokable::MarkReferences();
    number_of_locals->MarkReferences();
    maximum_number_of_stack_elements->MarkReferences();
    number_of_arguments->MarkReferences();
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

//VMMethod::VMMethod(MethodGenerationContext* mgenc) : VMArray(mgenc->), VMInvokable()
//{
//}

size_t VMMethod::GetOffset()
{
    return this->number_of_literals->GetEmbeddedInteger();
}

void VMMethod::SetLiteral(int index, VMObject* object)
{
    int fieldIndex = this->GetNumberOfFields() + 1 + index;
    //cout << "SetLiteral: " << index << "(" << fieldIndex << ")" << endl;
    //cout << "with: " << object->GetClass()->get_name()->GetStdString() << endl;
    //if (this->GetField(fieldIndex) == NULL)
        this->number_of_literals->SetEmbeddedInteger(number_of_literals->GetEmbeddedInteger()+1);
    this->SetField(fieldIndex, object);
}

VMObject* VMMethod::GetLiteral(int index)
{
    return this->GetField(this->GetNumberOfFields()+ 1 + index); //doesn't work like this... needs BC index
}

void VMMethod::invoke(VMFrame* frame)
{
    VMFrame* frm = _UNIVERSE->GetInterpreter()->PushNewFrame(this);
    frm->CopyArgumentsFrom(frame);
}

void VMMethod::set_holder_all(VMClass* hld)
{
    for (int i = 0; i < this->number_of_literals->GetEmbeddedInteger(); ++i)
    {
        VMObject* o = GetLiteral(i);
        if (dynamic_cast<VMInvokable*>(o) != NULL) 
        {
            ((VMInvokable*)o)->set_holder(hld);
        }
    }
}

VMObject* VMMethod::get_constant(int indx)
{
    //VMArray* 
    uint8_t bc = _BC[indx+1];
    //cout << "Get Constant at: " << bc  <<  " of "<< this->number_of_literals->GetEmbeddedInteger() << endl;
    if (bc >= this->number_of_literals->GetEmbeddedInteger()) {
        return NULL;
    }
    return this->GetLiteral(bc);
}

uint8_t VMMethod::get_bytecode(int indx)
{
    return _BC[indx];
}

int VMMethod::BytecodeLength() 
{
    return this->bc_length->GetEmbeddedInteger();
}

void VMMethod::set_bytecode(int indx, uint8_t val)
{
    _BC[indx] = val;
}

//VMMethod::~VMMethod() {}

