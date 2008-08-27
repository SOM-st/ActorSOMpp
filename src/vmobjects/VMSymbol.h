#pragma once
#ifndef VMSYMBOL_H_
#define VMSYMBOL_H_
//#include "OOObject.h"
#include "VMObject.h"

#include <iostream>
//#include <vector>
//#include "../misc/HeapAllocator.h"
//#include "../memory/Heap.h"

class VMSymbol : public VMObject//, public std::string{
{
public:
	VMSymbol();
	VMSymbol( const char* str );
	VMSymbol( const std::string& s );
	
	
	virtual ~VMSymbol();
	virtual int getObjectSize();
	char* GetChars();
	void SetString(const std::string&);
	void SetChars(const char*);
	std::string GetStdString();
	
	virtual void MarkReferences(){
		VMObject::MarkReferences();
	}
	
private:
	//vector<char, HeapAllocator<char> > chars;
	int string_length;
	//std::string chars;
	//char chars[0];
	char* chars;
};


#endif