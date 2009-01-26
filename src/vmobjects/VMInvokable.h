#pragma once

#ifndef VMINVOKABLE_H_
#define VMINVOKABLE_H_
#include "VMObject.h"
class VMSymbol;
class VMClass;
class VMFrame;

class VMInvokable : public VMObject {
public:
    VMInvokable(int nof = 0) : VMObject(nof+2) {};

    //virtual operator "()" to invoke the invokable
    virtual void      operator()(pVMFrame) = 0;

	virtual bool      IsPrimitive() const;
	virtual VMSymbol *GetSignature() const;
	virtual void      SetSignature(pVMSymbol sig);
	virtual VMClass  *GetHolder() const;
	virtual void      SetHolder(pVMClass hld);
    
protected:
	pVMSymbol signature;
	pVMClass holder;
};


#endif
