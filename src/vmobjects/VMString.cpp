#include <string.h>
#include <iostream>

#include "VMString.h"
#include "VMInteger.h"

//this macro could replace the chars member variable
//#define CHARS ((char*)&clazz+sizeof(pVMObject))

const int VMString::VMStringNumberOfFields = 0; 

VMString::VMString(const char* str) : VMObject(VMStringNumberOfFields) {
	//set the chars-pointer to point at the position of the first character
    chars = (char*)&chars+sizeof(char*);
	
    size_t i = 0;
	for (; i < strlen(str); ++i) {
		chars[i] = str[i];
	}
	chars[i] = '\0';
	
}


VMString::VMString( const StdString& s ): VMObject(VMStringNumberOfFields) {
    //set the chars-pointer to point at the position of the first character
	chars = (char*)&chars+sizeof(char*);
	size_t i = 0;
	for (; i < s.length(); ++i) {
		chars[i] = s[i];
	}
	chars[i] = '\0';
} 

int VMString::GetStringLength() const {
    //get the additional memory allocated by this object and substract one
    //for the '0' character and four for the char*
    return this->GetAdditionalSpaceConsumption() - 4 - 1;
}


StdString VMString::GetStdString() const {
    if (chars == 0) return StdString("");
	return StdString(chars);
}




