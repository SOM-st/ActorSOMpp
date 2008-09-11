#pragma once

#ifndef VMINVOKABLE_H_
#define VMINVOKABLE_H_
#include "VMObject.h"
class VMSymbol;
class VMClass;
class VMFrame;

class VMInvokable : public VMObject
{
public:
    VMInvokable(int nof = 0) : VMObject(nof+2) {};

	virtual void      invoke(VMFrame*) = 0;
	virtual bool      is_primitive();
	virtual VMSymbol *get_signature();
	virtual void      set_signature(VMSymbol* sig);
	virtual VMClass  *get_holder();
	virtual void      set_holder(VMClass* hld);
    //virtual void invoke_method(VMFrame*) = 0;

	virtual void MarkReferences();

protected:
	VMSymbol* signature;
	VMClass* holder;
};


#endif