#include "SymbolCompare.h"
#include "../vmobjects/VMString.h"

bool SymbolCompare::operator()(  VMString* s1,  VMString* s2 ) const {
        int minStrLength = s1->GetStringLength() < s2->GetStringLength() ?
                       s1->GetStringLength() : s2->GetStringLength();
        bool ret = strncmp( s1->GetChars(), s2->GetChars() , minStrLength ) < 0;
        return ret;
    }
