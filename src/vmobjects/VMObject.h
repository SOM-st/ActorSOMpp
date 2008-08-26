#pragma once
#ifndef VMOBJECT_H_
#define VMOBJECT_H_
//#include "OOObject.h"
#include "../misc/defs.h"
#include "../misc/HeapAllocator.h"
#include <vector>
#include <iostream>
#include "../memory/Heap.h"

//#include "VMClass.h"
class VMSymbol;
class VMClass;
class VMObject{

public:
	VMObject();
	virtual ~VMObject();

	virtual VMClass* GetClass();
	virtual void SetClass(VMClass* cl);
	virtual VMSymbol* GetFieldName(int index); 
	virtual int GetFieldIndex(VMSymbol* fieldName);
	virtual int GetNumberOfFields();
	virtual void SetNumberOfFields(int nof);
	virtual int GetDefaultNumberOfFields();
	virtual void Send(pString, VMObject*, int);
	virtual VMObject* GetField(int index);
	virtual void SetField(int index, VMObject* value);
	virtual void MarkReferences();
	
	//sizeof doesn't work polymorphic :(
	//every derived class must set objectSize = sizeof(CLASSNAME) in its constructor, so the heap and gc works
	virtual int getObjectSize() {return objectSize;}
	bool getGCField() {return gcfield;} ;
	void setGCField(bool value) { gcfield = value; } ;

	//num_bytes parameter is set by the compiler. This is a problem for VMString (and probably VMArray).
	//The new operator is called before the Constructor. Therefore we don't know how many chars the VMString
	//will have making it hard to allocate the memory for the chars. Options: 1. pass the number of chars to
	//the new operator; 2. set string length to an upper bound and have all VMString-Objects with the same size;
	//3. find something better...
	void *operator new( size_t num_bytes, Heap *heap, unsigned int additional_bytes = 0)
	{
		return heap->Allocate(num_bytes + additional_bytes);
	}

	void *operator new[](size_t num_bytes, Heap *heap)
	{
		return heap->Allocate(num_bytes);
	}

	void operator delete(void* self, Heap *heap, unsigned int additional_bytes)
	{
		heap->Free(self);
	}

	 void operator delete( void *self, Heap *heap) 
	 {
		 heap->Free(self); 
	 } 

	 void operator delete[]( void *self, Heap *heap ) 
	 {
		 heap->Free(self); 
	 } 
	/* uint16_t operator()(const VMObject &vmo) const
	{
		uint16_t sum=0;
		for (unsigned int i=0; i<str.size(); ++i)
			sum=(sum+str[i])%100;
		return sum;

	}*/
protected:
	int numberOfFields;
	bool gcfield : 1;
	//bool		 : 0; //forces alignment
	uint16_t hash;
	int objectSize;
	//VMObject** fields;
	VMClass* clazz;
	//VMObject* fields[0];
	//vector<VMObject, HeapAllocator<VMObject> > fields;
};


#endif