#pragma once
#ifndef VMMETHOD_H_
#define VMMETHOD_H_
//#include "OOObject.h"
#include "VMArray.h"
#include "VMObject.h"
#include "VMInteger.h"
#include "VMInvokable.h"
#include <iostream>
class MethodGenerationContext;
//#include "../memory/Heap.h"

class VMFrame;

class VMMethod : public VMArray, public VMInvokable{

public:
	VMMethod(int bc_count, int number_of_constants);
   // VMMethod(MethodGenerationContext* mgenc);
	virtual ~VMMethod();
    virtual int       get_number_of_locals();
    virtual void      set_number_of_locals(int nol);
    virtual int       get_maximum_number_of_stack_elements();
    virtual void      set_maximum_number_of_stack_elements(int stel);
    virtual int       get_number_of_arguments();
    virtual void      set_number_of_arguments(int noa);
    virtual int       get_number_of_bytecodes();
    virtual void      set_holder_all(VMClass* hld); 
    virtual VMObject *get_constant(int indx); 
    virtual uint8_t   get_bytecode(int indx); 
    virtual void      set_bytecode(int indx, uint8_t); 
	virtual void	  invoke(VMFrame* frame);
    virtual size_t    GetOffset();
	virtual void MarkReferences()
	{
		VMArray::MarkReferences();
		VMInvokable::MarkReferences();
	}

	
private:
    VMInteger* number_of_locals;
    VMInteger* maximum_number_of_stack_elements;
    VMInteger* bc_length;
    VMInteger* number_of_arguments;
	uint8_t* bc;	
};


#endif