
#pragma once
#ifndef VMSTRING_H_
#define VMSTRING_H_
//#include "OOObject.h"
#include "VMObject.h"

#include <iostream>
//#include <vector>
#include "../misc/HeapAllocator.h"
//#include "../memory/Heap.h"

class VMString : public VMObject//, public std::string{
{
public:
	VMString();
	VMString( const char* str );
	VMString( const string& s );
	//virtual ~VMString();

    char*       GetChars();
	std::string GetStdString();
    int         GetStringLength();

protected:
	char* chars;
};


#endif
