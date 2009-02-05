#pragma once
#ifndef VMMETHOD_H_
#define VMMETHOD_H_

#include <iostream>

#include "VMInvokable.h"

class VMArray;
class VMObject;
class VMInteger;
class MethodGenerationContext;
//#include "../memory/Heap.h"

class VMFrame;

class VMMethod :  public VMInvokable {

public:
	VMMethod(int bc_count, int number_of_constants, int nof = 0);
   // VMMethod(MethodGenerationContext* mgenc);
	//virtual ~VMMethod();
    virtual int       GetNumberOfLocals() const;
    virtual void      SetNumberOfLocals(int nol);
    virtual int       GetMaximumNumberOfStackElements() const;
    virtual void      SetMaximumNumberOfStackElements(int stel);
    virtual int       GetNumberOfArguments() const;
    virtual void      SetNumberOfArguments(int);
    virtual int       GetNumberOfBytecodes() const;
    virtual void      SetHolderAll(pVMClass hld); 
    virtual pVMObject GetConstant(int indx) const; 
    virtual uint8_t   GetBytecode(int indx) const; 
    virtual void      SetBytecode(int indx, uint8_t); 
	virtual void      MarkReferences();
    virtual int       GetNumberOfIndexableFields() const;

    void              SetIndexableField(int idx, pVMObject item);

    //VMArray Methods....
    
	
	pVMArray    CopyAndExtendWith(pVMObject) const;
	void        CopyIndexableFieldsTo(pVMArray) const;

    /// Methods are considered byte arrays with meta data.
    // So the index operator returns the bytecode at the index.
    uint8_t& operator[](int indx) const;

    //-----------VMInvokable-------------//
    //operator "()" to invoke the method
    virtual void	  operator()(pVMFrame frame);

	virtual void      SetSignature(pVMSymbol sig);


private:
    pVMObject   GetIndexableField(int idx) const;

    pVMInteger number_of_constants;
    pVMInteger number_of_locals;
    pVMInteger maximum_number_of_stack_elements;
    pVMInteger bc_length;
    pVMInteger number_of_arguments;

    static const int VMMethodNumberOfFields;
};


#endif
