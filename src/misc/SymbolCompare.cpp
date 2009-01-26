#include "SymbolCompare.h"
#include "../vmobjects/VMString.h"
#include <string.h>

bool SymbolCompare::operator()(  pVMString s1,  pVMString s2 ) const {
        int minStrLength = s1->GetStringLength() < s2->GetStringLength() ?
                       s1->GetStringLength() : s2->GetStringLength();
        bool ret = strncmp( s1->GetChars(), s2->GetChars() , minStrLength ) < 0;
        return ret;
    }
