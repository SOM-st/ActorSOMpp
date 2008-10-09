#pragma once
#ifndef VMSYMBOL_H_
#define VMSYMBOL_H_
//#include "OOObject.h"
#include "VMObject.h"

#include <iostream>
//#include <vector>
//#include "../misc/HeapAllocator.h"
//#include "../memory/Heap.h"
class VMInteger;
#include "VMString.h"
class VMSymbol : public VMString//, public std::string{
{
public:
	VMSymbol();
	VMSymbol( const char* str );
	VMSymbol( const std::string& s );
	virtual void SetString(const std::string&);
    virtual pString GetPlainString();
};


#endif
