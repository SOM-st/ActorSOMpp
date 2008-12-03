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
    virtual void      operator()(VMFrame*) = 0;

	virtual bool      IsPrimitive() const;
	virtual VMSymbol *GetSignature() const;
	virtual void      SetSignature(VMSymbol* sig);
	virtual VMClass  *GetHolder() const;
	virtual void      SetHolder(VMClass* hld);
    
protected:
	VMSymbol* signature;
	VMClass* holder;
};


#endif
