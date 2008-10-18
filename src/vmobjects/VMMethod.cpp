#include "VMMethod.h"
#include "../vm/Universe.h"
#include "VMFrame.h"
#include "../compiler/MethodGenerationContext.h"
#include "VMClass.h"
#include "VMSymbol.h"
#include "VMArray.h"
#include "VMObject.h"
#include "VMInteger.h"

#define _BC ((uint8_t*)&FIELDS[this->GetNumberOfFields() + this->GetNumberOfIndexableFields()])

#define theEntries(i) FIELDS[this->GetNumberOfFields()+i]

VMMethod::VMMethod(int bc_count, int number_of_constants, int nof) :  VMInvokable(nof + 5)//VMArray((bc_count/sizeof(VMObject*)) + number_of_constants ),
{
    _UNIVERSE->GetHeap()->StartUninterruptableAllocation();
    //objectSize += bc_count + number_of_constants*sizeof(VMObject*);
    bc_length = _UNIVERSE->NewInteger( bc_count );
    number_of_locals = _UNIVERSE->NewInteger(0);
    maximum_number_of_stack_elements = _UNIVERSE->NewInteger(0);
    number_of_arguments = _UNIVERSE->NewInteger(0);
    size = _UNIVERSE->NewInteger(number_of_constants);
    for (int i = 0; i < number_of_constants ; ++i)
    {
        this->SetIndexableField(i, nil_object);
    }
    _UNIVERSE->GetHeap()->EndUninterruptableAllocation();
}

void VMMethod::MarkReferences()
{
    if (gcfield) return;
    VMInvokable::MarkReferences();
    for (int i = 0 ; i < size->GetEmbeddedInteger() ; ++i)
	{
		if (theEntries(i) != NULL)
			theEntries(i)->MarkReferences();
	}
    /*number_of_locals->MarkReferences();
    maximum_number_of_stack_elements->MarkReferences();
    number_of_arguments->MarkReferences();
    size->MarkReferences();
    bc_length->MarkReferences();*/
}

int VMMethod::GetNumberOfLocals() 
{
    return number_of_locals->GetEmbeddedInteger(); 
}

void VMMethod::SetNumberOfLocals(int nol) 
{
    number_of_locals->SetEmbeddedInteger(nol); 
}

int VMMethod::GetMaximumNumberOfStackElements()
{
    return maximum_number_of_stack_elements->GetEmbeddedInteger(); 
}

void VMMethod::SetMaximumNumberOfStackElements(int stel) 
{
    maximum_number_of_stack_elements->SetEmbeddedInteger(stel); 
}

int VMMethod::GetNumberOfArguments() 
{
    return number_of_arguments->GetEmbeddedInteger(); 
}

void VMMethod::SetNumberOfArguments(int noa) 
{
    number_of_arguments->SetEmbeddedInteger(noa); 
}

int VMMethod::GetNumberOfBytecodes() 
{
    return bc_length->GetEmbeddedInteger();
}

void VMMethod::Invoke(VMFrame* frame)
{
    VMFrame* frm = _UNIVERSE->GetInterpreter()->PushNewFrame(this);
    frm->CopyArgumentsFrom(frame);
}

void VMMethod::SetHolderAll(VMClass* hld)
{
    for (int i = 0; i < this->GetNumberOfIndexableFields(); ++i)
    {
        VMObject* o = GetIndexableField(i);
        if (dynamic_cast<VMInvokable*>(o) != NULL) 
        {
            ((VMInvokable*)o)->SetHolder(hld);
        }
    }
}

VMObject* VMMethod::GetConstant(int indx)
{
    uint8_t bc = _BC[indx+1];
    if (bc >= this->GetNumberOfIndexableFields()) {
        cout << "Error: Constant index out of range" << endl;
        return NULL;
    }
    return this->GetIndexableField(bc);
}

uint8_t VMMethod::GetBytecode(int indx)
{
    return _BC[indx];
}


void VMMethod::SetBytecode(int indx, uint8_t val)
{
    _BC[indx] = val;
}

//VMArray Methods
VMArray* VMMethod::CopyAndExtendWith(VMObject* item)
{
    size_t fields = this->size->GetEmbeddedInteger();
	VMArray* result = _UNIVERSE->NewArray(fields+1);
    this->CopyIndexableFieldsTo(result);
	result->SetIndexableField(fields, item);
	return result;
}

VMObject* VMMethod::GetIndexableField(int idx)
{
    if (idx > size->GetEmbeddedInteger()-1 || idx < 0)  {
        cout << "Array index out of bounds: Accessing " << idx << ", but only " << size->GetEmbeddedInteger()-1;
        cout << " entries are available\n";
        _UNIVERSE->ErrorExit("Array index out of bounds exception");
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
        _UNIVERSE->ErrorExit("Array index out of bounds exception");
        //throw std::bad_exception();
    }
   	theEntries(idx) = item;
}

//VMMethod::~VMMethod() {}

