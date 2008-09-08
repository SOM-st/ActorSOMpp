#include "VMInvokable.h"

#include "VMSymbol.h"
#include "VMClass.h"
#include "Signature.h"
#include "VMMethod.h"

bool      VMInvokable::is_primitive() 
{
    return false;
};

VMSymbol *VMInvokable::get_signature() { 
    return signature; 
}

void      VMInvokable::set_signature(VMSymbol* sig) 
{ 
    signature = sig;
    
    if (!is_primitive()) ((VMMethod*)this)->set_number_of_arguments(Signature::GetNumberOfArguments(signature));
}

VMClass  *VMInvokable::get_holder() 
{
    return holder; 
}

void      VMInvokable::set_holder(VMClass* hld) 
{
    holder = hld; 
}
//virtual void invoke_method(VMFrame*) = 0;

void VMInvokable::MarkReferences() 
{
	signature->MarkReferences();
	holder->MarkReferences();
}
