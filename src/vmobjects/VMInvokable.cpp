#include "VMInvokable.h"

#include "VMSymbol.h"
#include "VMClass.h"

#include "VMMethod.h"


bool      VMInvokable::IsPrimitive() const {
    return false;
}


VMSymbol *VMInvokable::GetSignature() const { 
    return signature; 
}


void      VMInvokable::SetSignature(VMSymbol* sig)  { 
    signature = sig;
}


VMClass  *VMInvokable::GetHolder()  const {
    return holder; 
}


void      VMInvokable::SetHolder(VMClass* hld) {
    holder = hld; 
}
//virtual void invoke_method(VMFrame*) = 0;
