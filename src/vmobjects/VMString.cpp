#include "VMString.h"

VMString::VMString() : VMObject()
{
	objectSize += sizeof(vector<char, HeapAllocator<char> >);
	vector<char, HeapAllocator<char> > v = vector<char, HeapAllocator<char> >(HeapAllocator<char>(Universe::GetUniverse()->GetHeap()));
}

void VMString::FromCString(const char* text)
{
	int i = 0;
	do
	{
		chars.push_back(text[i]);
	} while (text[i++] != '\0');
}

char* VMString::ToCString()
{
	char* c = (char*)malloc(chars.size());
	int i = 0;
	do {
		c[i] = chars[i];
	} while(c[i++] != '\0');
	return c;
}

int VMString::getObjectSize()
{
	return objectSize + chars.size();
}

VMString::~VMString() {}

