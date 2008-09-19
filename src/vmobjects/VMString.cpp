#include "VMString.h"
#include "VMInteger.h"

VMString::VMString() : VMObject(1)//, std::string()
{
	chars = 0;
    string_length = _UNIVERSE->new_integer(0);
	//objectSize = sizeof(VMString);
	//chars = vector<char, HeapAllocator<char> >(HeapAllocator<char>(Universe::GetUniverse()->GetHeap()));
}



VMString::VMString(const char* str) : VMObject(1)//, std::string()
{
	chars = (char*)&chars+sizeof(char*);
	objectSize += strlen(str) + 1; //set actual object_size
	string_length = _UNIVERSE->new_integer(strlen(str));
    size_t i = 0;
	for (; i < strlen(str); ++i) {
		chars[i] = str[i];
	}
	chars[i] = '\0';
	//chars = vector<char, HeapAllocator<char> >(HeapAllocator<char>(Universe::GetUniverse()->GetHeap()));
}
VMString::VMString( const string& s ): VMObject(1)
{
	chars = (char*)&chars+sizeof(char*);
	objectSize += s.length() + 1;
	string_length = _UNIVERSE->new_integer(s.length());
    size_t i = 0;
	for (; i < s.length(); ++i) {
		chars[i] = s[i];
	}
	chars[i] = '\0';
}

void VMString::MarkReferences(){
	VMObject::MarkReferences();
    string_length->MarkReferences();
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
    return string_length->GetEmbeddedInteger();
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

int VMString::getObjectSize()
{
	return objectSize;// + chars.size();
}

//VMString::~VMString() {}

