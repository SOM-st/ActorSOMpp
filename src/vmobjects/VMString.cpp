#include <string.h>
#include <iostream>

#include "VMString.h"
#include "VMInteger.h"

//this macro could replace the chars member variable
//#define CHARS ((char*)&clazz+sizeof(pVMObject))

const int VMString::VMStringNumberOfFields = 0; 

VMString::VMString(const char* str) : VMObject(VMStringNumberOfFields) { //, StdString()
	chars = (char*)&chars+sizeof(char*);
	//objectSize += sizeof(char*) + strlen(str) + 1; //set actual object_size
	//string_length = _UNIVERSE->NewInteger(strlen(str));
    size_t i = 0;
	for (; i < strlen(str); ++i) {
		chars[i] = str[i];
	}
	chars[i] = '\0';
	
}


VMString::VMString( const string& s ): VMObject(VMStringNumberOfFields) {
	chars = (char*)&chars+sizeof(char*);
	//objectSize += sizeof(char*) + s.length() + 1;
	//string_length = _UNIVERSE->NewInteger(s.length());
    size_t i = 0;
	for (; i < s.length(); ++i) {
		chars[i] = s[i];
	}
	chars[i] = '\0';
} 


void VMString::SetString(const StdString& str) {
	if (str.length() > (size_t)GetStringLength()) {
		//realloc?
        cout << "Problem: trying to SetString of a VMSymbol"
             << " that doesn't have enough mem" << endl;
	} else {
        //char* chars = this->GetChars();
        unsigned int i;
		for (i = 0; i < (size_t)str.length(); i++) {
			chars[i] = str[i];
		}
		chars[i] = '\0';
	}
}


int VMString::GetStringLength() const {
    //cout << objectSize << endl << sizeof(VMString) << endl;
    //get the additional memory allocated by this object and substract one
    //for the '0' character and four for the char*
    return this->GetAdditionalSpaceConsumption() - 4 - 1;
    //int length = 0;
    ////if (chars != 0) {
    //    while(chars[length++] != '\0');
    //    if (length != 0)
    //        --length;
    ////}
    //    cout << "length: " << length << endl;
    //    cout << "addsp: " << this->GetAdditionalSpaceConsumption() << endl << endl;
    //return length;
}


StdString VMString::GetStdString() const {
    if (chars == 0) return StdString("");
	return StdString(chars);
}




