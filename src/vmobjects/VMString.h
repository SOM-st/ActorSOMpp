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
	/*VMString( const string& s );
	VMString( size_type length, const char& ch );
	VMString( const char* str );
	VMString( const char* str, size_type length );
	VMString( const string& str, size_type index, size_type length );*/

	virtual ~VMString();
	virtual int getObjectSize();
	char* GetChars();
	std::string GetStdString();
	/*virtual pString GetClass(); //TODO: change string to VMClass
	virtual void SetClass(pString className);
	virtual pString GetFieldName(int index); //TODO: change string to VMSymbol
	virtual int GetFieldIndex(pString fieldName);
	virtual int GetNumberOfFields();
	virtual void SetNumberOfFields(int nof);
	virtual int GetDefaultNumberOfFields();
	virtual void Send(pString, VMObject*, int);
	virtual VMObject* GetField(int index);
	virtual void SetField(int index, VMObject* value);*/
	virtual void MarkReferences(){
		VMObject::MarkReferences();
	}
	
private:
	//vector<char, HeapAllocator<char> > chars;
	//char* chars;
	//char chars[0];
	//int string_length;
	char* chars;
};


#endif