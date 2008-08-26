#include "VMString.h"

VMString::VMString() : VMObject()//, std::string()
{
	chars = 0;
	objectSize = sizeof(VMString);
	//chars = vector<char, HeapAllocator<char> >(HeapAllocator<char>(Universe::GetUniverse()->GetHeap()));
}

VMString::VMString(const char* str) : VMObject()//, std::string()
{
	chars = (char*)&chars+sizeof(char*);
	objectSize = sizeof(VMString) + strlen(str) + 1;
	int string_length = strlen(str);
	for (int i = 0; i < string_length; i++) {
		chars[i] = str[i];
	}
	chars[string_length] = '\0';
	//chars = vector<char, HeapAllocator<char> >(HeapAllocator<char>(Universe::GetUniverse()->GetHeap()));
}
VMString::VMString( const string& s ): VMObject()
{
	chars = (char*)&chars+sizeof(char*);
	objectSize = sizeof(VMString) + s.length() + 1;
	int string_length = s.length();
	for (int i = 0; i < string_length; i++) {
		chars[i] = s[i];
	}
	chars[string_length] = '\0';
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

std::string VMString::GetStdString()
{
	return std::string(chars);
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

VMString::~VMString() {}

