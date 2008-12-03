#include "Signature.h"
#include "VMSymbol.h"


int Signature::GetNumberOfArguments(VMSymbol *sig) {
    // check default binaries
    if(Signature::IsBinary(sig)) return 2;
    else {
        StdString str = sig->GetStdString();
        
        // colons in str
        int num_colons =0;
        
        // search the str
        for(unsigned int i=0 ; i<=str.length(); ++i) 
            if(str[i]==':')
                // additional colon found
                num_colons++;
        
        // The number of arguments is equal to the number of colons plus one
        // (->> SELF)
        return num_colons + 1;        
    }    
}


bool Signature::IsBinary(VMSymbol *sig) {
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
