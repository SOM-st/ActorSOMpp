#include "VMString.h"
#include "VMInteger.h"
#include <string.h>

VMString::VMString() : VMObject(0)//, std::string()
{
	chars = 0;
    //objectSize += sizeof(char*);
    //string_length = _UNIVERSE->new_integer(0);
	//objectSize = sizeof(VMString);
	//chars = vector<char, HeapAllocator<char> >(HeapAllocator<char>(Universe::GetUniverse()->GetHeap()));
}



VMString::VMString(const char* str) : VMObject(0)//, std::string()
{
	chars = (char*)&chars+sizeof(char*);
	//objectSize += sizeof(char*) + strlen(str) + 1; //set actual object_size
	//string_length = _UNIVERSE->new_integer(strlen(str));
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
	//string_length = _UNIVERSE->new_integer(s.length());
    size_t i = 0;
	for (; i < s.length(); ++i) {
		chars[i] = s[i];
	}
	chars[i] = '\0';
}

//VMString::VMString( size_type length, const char& ch ): VMObject(), std::string(length, ch)
//{
//	objectSize = sizeof(VMString) + length;
//}
//VMString::VMString( const char* str ): VMObject(), std::string(str)
//{
//	objectSize = sizeof(VMString) + strlen(str);
//}
//VMString::VMString( const char* str, size_type length ): VMObject(), std::string(str, length)
//{
//	objectSize = sizeof(VMString) + length;
//}
//VMString::VMString( const string& str, size_type index, size_type length ): VMObject(), std::string(str, index, length)
//{
//	objectSize = sizeof(VMString) + length;
//}

int VMString::GetStringLength()
{
    int length = 0;
    if (chars != 0) {
        while(chars[length++] != '\0');
        if (length != 0)
            --length;
    }
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
	/*char* c = (char*)malloc(string_length+1);
	int i = 0;
	do {
		c[i] = chars[i];
	} while(++i < string_length + 1);
	return c;
	return "";*/
}


//VMString::~VMString() {}

