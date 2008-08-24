#pragma once
#ifndef VMMETHOD_H_
#define VMMETHOD_H_
//#include "OOObject.h"
#include "VMObject.h"

#include <iostream>
//#include "../memory/Heap.h"

class VMMethod : public VMObject{

public:
	VMMethod();
	virtual ~VMMethod();
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
	
	int32_t getGCField() {return gcfield;} ;
	void setGCField(int32_t value) { gcfield = value; } ;

	void *operator new( unsigned int num_bytes, Heap *heap)
	{
		return heap->Allocate(num_bytes);
	}

	 void operator delete( void *self, Heap *heap ) 
	 {
		 heap->Free(self); 
	 } 

	 uint16_t operator()(const std::string &str) const
	{
		uint16_t sum=0;
		for (int i=0; i<str.size(); ++i)
			sum=(sum+str[i])%100;
		return sum;

	}*/
private:
	//for testing mem alloc
	int32_t a[10];
	
};


#endif