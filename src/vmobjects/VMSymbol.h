#pragma once
#ifndef VMSYMBOL_H_
#define VMSYMBOL_H_

#include <iostream>

#include "VMString.h"
#include "VMObject.h"

class VMInteger;

class VMSymbol : public VMString {

public:
	VMSymbol( const char* str );
	VMSymbol( const StdString& s );
    virtual StdString GetPlainString() const;

};


#endif
