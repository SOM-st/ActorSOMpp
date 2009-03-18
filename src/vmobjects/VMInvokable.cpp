#include "VMInvokable.h"

#include "VMSymbol.h"
#include "VMClass.h"



bool      VMInvokable::IsPrimitive() const {
    return false;
}


pVMSymbol VMInvokable::GetSignature() const { 
    return signature; 
}


void      VMInvokable::SetSignature(pVMSymbol sig)  { 
    signature = sig;
}


pVMClass VMInvokable::GetHolder()  const {
    return holder; 
}


void      VMInvokable::SetHolder(pVMClass hld) {
    holder = hld; 
}
