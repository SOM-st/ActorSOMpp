#include "VMSymbol.h"
#include "VMInteger.h"
#include "VMString.h"

VMSymbol::VMSymbol() : VMString()//, std::string()
{
	//chars = 0;
	//objectSize = sizeof(VMSymbol);
    /*if (sizeof(VMSymbol) == sizeof(VMString))
        cout << "si" << endl;
    else cout << "oh no" << endl;*/
	//chars = vector<char, HeapAllocator<char> >(HeapAllocator<char>(Universe::GetUniverse()->GetHeap()));
}

VMSymbol::VMSymbol(const char* str) : VMString(str)//, std::string()
{
	//VMSymbol(std::string(str));
	//chars = vector<char, HeapAllocator<char> >(HeapAllocator<char>(Universe::GetUniverse()->GetHeap()));
}

VMSymbol::VMSymbol( const std::string& s ): VMString(s)
{
	/*chars = (char*)&chars+sizeof(char*);
	objectSize = sizeof(VMSymbol) + s.length() + 1;
	string_length = _UNIVERSE->new_integer(s.length());
	SetString(s);*/
}

void VMSymbol::SetString(const std::string& str)
{
	if (str.length() > (size_t)GetStringLength()) {
		//realloc?
        cout << "Problem: trying to SetString of a VMSymbol that doesn't have enough mem" << endl;
	} else {
        int i;
		for (i = 0; i < (size_t)str.length(); i++) {
			chars[i] = str[i];
		}
		chars[i] = '\0';
	}
}

pString VMSymbol::GetPlainString()
{
    
    pString plain_string;
    
    for(size_t i=0; i <= (size_t)this->GetStringLength(); i++) {
        char c = this->GetChars()[i];
        switch (c) {
            case '~':
                plain_string += "tilde"; 
                break;
            case '&':
                plain_string += "and"; 
                break;
            case '|':
                plain_string += "bar";
                break;
            case '*':
                plain_string += "star";
                break;
            case '/':
                plain_string += "slash";
                break;
            case '@':
                plain_string += "at";
                break;
            case '+':
                plain_string += "plus";
                break;
            case '-':
                plain_string += "minus";
                break;
            case '=':
                plain_string += "equal";
                break;     
            case '>':
                plain_string += "greaterthan";
                break;
            case '<':
                plain_string += "lowerthan";
                break;
            case ',':
                plain_string += "comma";
                break;
            case '%':
                plain_string += "percent";
                break;
            case '\\':
                plain_string += "backslash";
                break;
            case ':':
                plain_string += '_';
                break;
        #ifdef EXPERIMENTAL
            case ' ':
                SEND(self->plain_string, concatChar, '_';
                break;
        #endif
            default:
                if (c != 0)
                    plain_string += c;
                break;
        }
    }
    return plain_string;
}


/*
void VMSymbol::MarkReferences(){
	VMString::MarkReferences();
    //string_length->MarkReferences();
}

void VMSymbol::SetChars(const char* str)
{
	SetString(std::string(str));
}

std::string VMSymbol::GetStdString()
{
	return std::string(chars);
}

char* VMSymbol::GetChars()
{
	return chars;
}

int VMSymbol::getObjectSize()
{
	return objectSize;// + chars.size();
}
*/

