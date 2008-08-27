#include "VMSymbol.h"

VMSymbol::VMSymbol() : VMObject()//, std::string()
{
	chars = 0;
	objectSize = sizeof(VMSymbol);
	//chars = vector<char, HeapAllocator<char> >(HeapAllocator<char>(Universe::GetUniverse()->GetHeap()));
}

VMSymbol::VMSymbol(const char* str) : VMObject()//, std::string()
{
	VMSymbol(std::string(str));
	//chars = vector<char, HeapAllocator<char> >(HeapAllocator<char>(Universe::GetUniverse()->GetHeap()));
}

VMSymbol::VMSymbol( const std::string& s ): VMObject()
{
	chars = (char*)&chars+sizeof(char*);
	objectSize = sizeof(VMSymbol) + s.length() + 1;
	string_length = s.length();
	SetString(s);
}

void VMSymbol::SetString(const std::string& str)
{
	if (str.length() > string_length) {
		//realloc?
	} else {
		for (int i = 0; i < string_length; i++) {
			chars[i] = str[i];
		}
		chars[string_length] = '\0';
	}
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

VMSymbol::~VMSymbol() {}

