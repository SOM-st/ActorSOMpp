#include "VMString.h"
#include "VMInteger.h"
#include <string.h>
#include <iostream>

//this macro could replace the chars member variable
//#define CHARS ((char*)&clazz+sizeof(VMObject*))

VMString::VMString(const char* str) : VMObject(0)//, std::string()
{
	chars = (char*)&chars+sizeof(char*);
	//objectSize += sizeof(char*) + strlen(str) + 1; //set actual object_size
	//string_length = _UNIVERSE->NewInteger(strlen(str));
    size_t i = 0;
	for (; i < strlen(str); ++i) {
		chars[i] = str[i];
	}
	chars[i] = '\0';
	//chars = vector<char, HeapAllocator<char> >(HeapAllocator<char>(Universe::GetUniverse()->GetHeap()));
}
VMString::VMString( const string& s ): VMObject(0)
{
	chars = (char*)&chars+sizeof(char*);
	//objectSize += sizeof(char*) + s.length() + 1;
	//string_length = _UNIVERSE->NewInteger(s.length());
    size_t i = 0;
	for (; i < s.length(); ++i) {
		chars[i] = s[i];
	}
	chars[i] = '\0';
}

void VMString::SetString(const std::string& str)
{
	if (str.length() > (size_t)GetStringLength()) {
		//realloc?
        cout << "Problem: trying to SetString of a VMSymbol that doesn't have enough mem" << endl;
	} else {
        //char* chars = this->GetChars();
        unsigned int i;
		for (i = 0; i < (size_t)str.length(); i++) {
			chars[i] = str[i];
		}
		chars[i] = '\0';
	}
}

int VMString::GetStringLength()
{
    //cout << objectSize << endl << sizeof(VMString) << endl;
    int length = 0;
    //if (chars != 0) {
        while(chars[length++] != '\0');
        if (length != 0)
            --length;
    //}
   //cout << length << endl << endl;
    return length;
}

std::string VMString::GetStdString()
{
    if (chars == 0) return pString("");
	return pString(chars);
}

char* VMString::GetChars()
{
	return chars;
}


