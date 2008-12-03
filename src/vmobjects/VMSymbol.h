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

class VMSymbol : public VMString { //, public StdString{

public:
	VMSymbol( const char* str );
	VMSymbol( const StdString& s );
	//virtual void SetString(const StdString&);
    virtual StdString GetPlainString() const;
};


#endif
