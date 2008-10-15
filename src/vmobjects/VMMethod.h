#pragma once
#ifndef VMMETHOD_H_
#define VMMETHOD_H_
//#include "OOObject.h"
class VMArray;
class VMObject;
class VMInteger;
#include "VMInvokable.h"
#include <iostream>
class MethodGenerationContext;
//#include "../memory/Heap.h"

class VMFrame;

class VMMethod :  public VMInvokable{//public VMArray,

public:
	VMMethod(int bc_count, int number_of_constants, int nof = 0);
   // VMMethod(MethodGenerationContext* mgenc);
	//virtual ~VMMethod();
    virtual int       GetNumberOfLocals();
    virtual void      SetNumberOfLocals(int nol);
    virtual int       GetMaximumNumberOfStackElements();
    virtual void      SetMaximumNumberOfStackElements(int stel);
    virtual int       GetNumberOfArguments();
    virtual void      SetNumberOfArguments(int);
    virtual int       GetNumberOfBytecodes();
    virtual void      SetHolderAll(VMClass* hld); 
    virtual VMObject* GetConstant(int indx); 
    virtual uint8_t   GetBytecode(int indx); 
    virtual void      SetBytecode(int indx, uint8_t); 
	virtual void	  Invoke(VMFrame* frame);
	virtual void      MarkReferences();

    //VMArray Methods....
    void        SetIndexableField(int idx, VMObject* item);
	VMObject*   GetIndexableField(int idx);
	int         GetNumberOfIndexableFields();
	VMArray*    CopyAndExtendWith(VMObject*);
	void        CopyIndexableFieldsTo(VMArray*);

private:
    VMInteger* size;
    VMInteger* number_of_locals;
    VMInteger* maximum_number_of_stack_elements;
    VMInteger* bc_length;
    VMInteger* number_of_arguments;
};


#endif
