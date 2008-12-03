
#pragma once
#ifndef VMSTRING_H_
#define VMSTRING_H_
//#include "OOObject.h"
#include "VMObject.h"

class VMString : public VMObject {
public:
	VMString( const char* str );
	VMString( const string& s );
	//virtual ~VMString();

    inline char*       GetChars() const;
	StdString GetStdString() const;
    int         GetStringLength() const;

    void        SetString(const StdString&);
protected:
    //this could be replaced by the CHARS macro in VMString.cpp
    //in order to decrease the object size
	char* chars; 
};

char* VMString::GetChars() const {
	return chars;
}
#endif
