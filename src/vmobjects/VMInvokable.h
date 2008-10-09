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

	virtual void      Invoke(VMFrame*) = 0;
	virtual bool      IsPrimitive();
	virtual VMSymbol *GetSignature();
	virtual void      SetSignature(VMSymbol* sig);
	virtual VMClass  *GetHolder();
	virtual void      SetHolder(VMClass* hld);
    //virtual void invoke_method(VMFrame*) = 0;

protected:
	VMSymbol* signature;
	VMClass* holder;
};


#endif
