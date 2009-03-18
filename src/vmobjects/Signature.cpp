#include "Signature.h"
#include "VMSymbol.h"


int Signature::GetNumberOfArguments(pVMSymbol sig) {
    // check default binaries
    if(Signature::IsBinary(sig)) return 2;
    else {
        StdString str = sig->GetStdString();
        
        // colons in str
        int numColons =0;
        
        // search the str
        for(unsigned int i=0 ; i<=str.length(); ++i) 
            if(str[i]==':')
                // additional colon found
                numColons++;
        
        // The number of arguments is equal to the number of colons plus one
        // (->> SELF)
        return numColons + 1;        
    }    
}


bool Signature::IsBinary(pVMSymbol sig) {
    StdString sigstr = sig->GetStdString();
    switch(sigstr[0]) {
            case '~' :
            case '&' :
            case '|' :
            case '*' :
            case '/' :
            case '@' :
            case '+' :
            case '-' :
            case '=' :
            case '>' :
            case '<' :
            case ',' :
            case '%' :
            case '\\':        
                return true;
            default: break;
    }
    return false;
}
