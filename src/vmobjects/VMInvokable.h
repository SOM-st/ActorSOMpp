#pragma once

#ifndef VMINVOKABLE_H_
#define VMINVOKABLE_H_
#include "VMObject.h"
#include "VMSymbol.h"
#include "VMClass.h"
class VMFrame;

class VMInvokable
{
public:
	virtual void      invoke(VMFrame*) = 0;
	virtual bool      is_primitive() {return false;} ;
	virtual VMSymbol *get_signature() { return signature; };
	virtual void      set_signature(VMSymbol* sig) { signature = sig; };
	virtual VMClass  *get_holder() { return holder; };
	virtual void      set_holder(VMClass* hld) { holder = hld; };
    //virtual void invoke_method(VMFrame*) = 0;

	virtual void MarkReferences() {
		signature->MarkReferences();
		holder->MarkReferences();
	}

protected:
	VMSymbol* signature;
	VMClass* holder;
};

#endif