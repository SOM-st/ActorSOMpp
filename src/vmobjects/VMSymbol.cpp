#include <sstream>

#include "VMSymbol.h"
#include "VMInteger.h"


VMSymbol::VMSymbol(const char* str) : VMString(str) {
}


VMSymbol::VMSymbol( const StdString& s ): VMString(s) {
}


StdString VMSymbol::GetPlainString() const {
    ostringstream str;
    char* chars = this->GetChars();
    for(size_t i=0; i <= (size_t)this->GetStringLength(); i++) {
        char c = chars[i];
        switch (c) {
            case '~':
                str << "tilde";
                break;
            case '&':
                str << "and";
                break;
            case '|':
                str << "bar";
                break;
            case '*':
                str << "star";
                break;
            case '/':
                str << "slash";
                break;
            case '@':
                str << "at";
                break;
            case '+':
                str << "plus";
                break;
            case '-':
                str << "minus";
                break;
            case '=':
                str << "equal";
                break;     
            case '>':
                str << "greaterthan" ;
                break;
            case '<':
                str << "lowerthan";
                break;
            case ',':
                str << "comma";
                break;
            case '%':
                str << "percent";
                break;
            case '\\':
                str << "backslash";
                break;
            case ':':
                str << '_';
                break;
        #ifdef EXPERIMENTAL
            case ' ':
                SEND(self->plain_string, concatChar, '_';
                break;
        #endif
            default:
                if (c != 0) {
                    str << c;
                }
                break;
        }
    }
    StdString st = str.str();
    //st[0] = toupper(st[0]);
    return st;
}

