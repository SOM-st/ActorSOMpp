#pragma once
#ifndef VMSTRING_H_
#define VMSTRING_H_
//#include "OOObject.h"
#include "VMObject.h"

#include <iostream>
#include <vector>
#include "../misc/HeapAllocator.h"
//#include "../memory/Heap.h"

class VMString : public VMObject{

public:
	VMString();
	virtual ~VMString();
	virtual int getObjectSize();
	char* ToCString();
	void FromCString(const char*);
	/*virtual pString GetClass(); //TODO: change string to VMClass
	virtual void SetClass(pString className);
	virtual pString GetFieldName(int index); //TODO: change string to VMSymbol
	virtual int GetFieldIndex(pString fieldName);
	virtual int GetNumberOfFields();
	virtual void SetNumberOfFields(int nof);
	virtual int GetDefaultNumberOfFields();
	virtual void Send(pString, VMObject*, int);
	virtual VMObject* GetField(int index);
	virtual void SetField(int index, VMObject* value);
	virtual void MarkReferences();
	*/
private:
	vector<char, HeapAllocator<char> > chars;
};


#endif