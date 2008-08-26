#pragma once
#ifndef VMOBJECT_H_
#define VMOBJECT_H_
//#include "OOObject.h"
#include "../misc/defs.h"

#include <iostream>
#include "../memory/Heap.h"

class VMObject{

public:
	VMObject();
	virtual ~VMObject();
	virtual pString GetClass(); //TODO: change string to VMClass
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
	
	//sizeof doesn't work polymorphic :(
	virtual int getObjectSize() {return objectSize;}
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
		for (unsigned int i=0; i<str.size(); ++i)
			sum=(sum+str[i])%100;
		return sum;

	}
protected:
	int numberOfFields;
	uint32_t gcfield;
	uint16_t hash;
	int objectSize;
	
};


#endif