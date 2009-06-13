#pragma once
#ifndef VMSTRING_H_
#define VMSTRING_H_

/*
 *
 *
Copyright (c) 2007 Michael Haupt, Tobias Pape, Arne Bergmann
Software Architecture Group, Hasso Plattner Institute, Potsdam, Germany
http://www.hpi.uni-potsdam.de/swa/

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
  */

#include <string.h>

#include "VMObject.h"

class VMString : public VMObject {
public:
	VMString( const char* str );
	VMString( const StdString& s );

    inline char*       GetChars() const;
	StdString GetStdString() const;
    int         GetStringLength() const;

    virtual bool IsImmutable() {
        return true;
    }
    
    virtual ImmutableTypes GetSerializationTag() {
        return StringTag;
    }
    
    
    virtual size_t GetSerializedSize() {
        return VMObject::GetSerializedSize()
        + sizeof(size_t)
        + strlen(chars) + 1;
    }
    
    virtual void* Serialize(void* buffer) {
        size_t len = strlen(chars);
        
        buffer = VMObject::Serialize(buffer);
        
        *(size_t*)buffer = len;
        
        char* serialized_chars = (char*)((intptr_t)buffer + sizeof(size_t));
        strncpy(serialized_chars, chars, len + 1);
        
        return (void*)((intptr_t)serialized_chars + len + 1);
    }
    
    static void* Deserialize(void* buffer, size_t& length, char*& str) {
        length = *(size_t*)buffer;
        
        str = (char*)((intptr_t)buffer + sizeof(size_t));
        
        return (void*)((intptr_t)str + length + 1);
    }
    
protected:
    
    //this could be replaced by the CHARS macro in VMString.cpp
    //in order to decrease the object size
	char* chars; 

private:
    static const int VMStringNumberOfFields;
};

char* VMString::GetChars() const {
	return chars;
}
#endif
