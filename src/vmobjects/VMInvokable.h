#pragma once

#ifndef VMINVOKABLE_H_
#define VMINVOKABLE_H_

#include "ObjectFormats.h"

class VMSymbol;
class VMClass;
class VMFrame;

class VMInvokable {
public:
    //VMInvokable(){};
    //virtual operator "()" to invoke the invokable
    virtual void      operator()(pVMFrame) = 0;

    virtual bool      IsPrimitive() const = 0;
	virtual pVMSymbol GetSignature() const = 0;
	virtual void      SetSignature(pVMSymbol sig) = 0;
	virtual pVMClass  GetHolder() const = 0;
	virtual void      SetHolder(pVMClass hld) = 0;
    
//protected:
//	pVMSymbol signature;
//	pVMClass holder;
};


#endif
