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
    VMObject(int number_of_fields);
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

	//num_bytes parameter is set by the compiler.
    //parameter additional_bytes (a_b) is used for:
    //  - fields in VMObject, a_b must be set to (numberOfFields*sizeof(VMObject*))
    //  - chars in VMString/VMSymbol, a_b must be set to (Stringlength + 1)
    //  - array size in VMArray; a_b must be set to (size_of_array*sizeof(VMObect*))
    //  - fields in VMMethod, a_b must be set to (number_of_bc + number_of_csts*sizeof(VMObject*))
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
	
    VMObject** fields;
	VMClass* clazz;
};


#endif