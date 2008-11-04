#include "VMInvokable.h"

#include "VMSymbol.h"
#include "VMClass.h"
#include "Signature.h"
#include "VMMethod.h"


bool      VMInvokable::IsPrimitive() 
{
    return false;
}


VMSymbol *VMInvokable::GetSignature() { 
    return signature; 
}


void      VMInvokable::SetSignature(VMSymbol* sig) 
{ 
    signature = sig;
    
    if (!IsPrimitive()) 
        ((VMMethod*)this)->SetNumberOfArguments(Signature::GetNumberOfArguments(signature));
}


VMClass  *VMInvokable::GetHolder() 
{
    return holder; 
}


void      VMInvokable::SetHolder(VMClass* hld) 
{
    holder = hld; 
}
//virtual void invoke_method(VMFrame*) = 0;
