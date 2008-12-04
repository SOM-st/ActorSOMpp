#pragma once
#ifndef VMOBJECT_H_
#define VMOBJECT_H_
//#include "OOObject.h"
#include "../misc/defs.h"
#include <vector>
#include <iostream>
#include "../memory/Heap.h"
#include "../vm/Universe.h"

#include "ObjectFormats.h"
//#include "VMClass.h"
class VMSymbol;
class VMClass;

#define FIELDS ((VMObject**)&clazz)

/*
 **************************VMOBJECT****************************
 * __________________________________________________________ *
 *| vtable*          |   0x00 - 0x03                         |*
 *|__________________|_______________________________________|*
 *| hash             |   0x04 - 0x07                         |*
 *| objectSize       |   0x08 - 0x0b                         |*
 *| numberOfFields   |   0x0c - 0x0f                         |*
 *| gcField          |   0x10 - 0x13 (because of alignment)  |*
 *| clazz            |   0x14 - 0x17                         |*
 *|__________________|___0x18________________________________|*
 *                                                            *
 **************************************************************
 */

class VMObject {

public:
    /* Constructor */
    VMObject(int numberOfFields = 0);
    
    /* Virtual member functions */
	virtual VMClass*    GetClass() const;
	virtual void        SetClass(VMClass* cl);
	virtual VMSymbol*   GetFieldName(int index) const; 
	virtual int         GetFieldIndex(VMSymbol* fieldName) const;
	virtual int         GetNumberOfFields() const;
	virtual void        SetNumberOfFields(int nof);
	virtual int         GetDefaultNumberOfFields() const;
	virtual void        Send(StdString, VMObject**, int);
	virtual VMObject*   GetField(int index) const;
    virtual void        Assert(bool value) const;
	virtual void        SetField(int index, VMObject* value);
	virtual void        MarkReferences();

    virtual void        IncreaseGCCount() {};
    virtual void        DecreaseGCCount() {};

    int32_t     GetHash() const { return hash; };
    int         GetObjectSize() const;
	bool        GetGCField() const;
	void        SetGCField(bool value);
    void        SetObjectSize(size_t size) {objectSize = size; } ;
	
    /* Operators */

    /**
     * usage: new( <heap> [, <additional_bytes>] ) VMObject( <constructor params> )
     * num_bytes parameter is set by the compiler.
     * parameter additional_bytes (a_b) is used for:
     *   - fields in VMObject, a_b must be set to (numberOfFields*sizeof(VMObject*))
     *   - chars in VMString/VMSymbol, a_b must be set to (Stringlength + 1)
     *   - array size in VMArray; a_b must be set to (size_of_array*sizeof(VMObect*))
     *   - fields in VMMethod, a_b must be set to (number_of_bc + number_of_csts*sizeof(VMObject*))
     */
	void *operator new( size_t num_bytes, Heap *heap, 
                        unsigned int additional_bytes = 0) {
        /*if (num_bytes == 24) {
            cout << "hier";
        }
        cout << "Allocating " << num_bytes << "+" << additional_bytes << " = " << num_bytes + additional_bytes << "Bytes" <<endl;*/
        void* mem = (void*)heap->AllocateObject(num_bytes + additional_bytes);
        return mem;
	}

	void *operator new[](size_t num_bytes, Heap *heap) {
		return heap->Allocate(num_bytes);
	}

	void operator delete(void* self, Heap *heap, 
                         unsigned int /*additional_bytes*/) {
        int size = ((VMObject*)self)->GetObjectSize();
		heap->Free(self, size);
	}

	 void operator delete( void *self, Heap *heap) {
         int size = ((VMObject*)self)->GetObjectSize();
		 heap->Free(self, size); 
	 } 

	 void operator delete[]( void *self, Heap *heap ) {
		 heap->Free(self); 
	 }

     VMObject* operator->() {
         cout << "->" << endl;
         return this;
     }
	
protected:
    int GetAdditionalSpaceConsumption() const;
    //VMObject essentials
	int32_t     hash;
    int32_t     objectSize; //set by the heap at allocation time
    int32_t     numberOfFields;
    bool        gcfield;

    //VMObject** FIELDS;
    //Start of fields. All members beyond this point are indexable 
    //through FIELDS-macro instead of the member above. 
    //So clazz == FIELDS[0]
	VMClass*    clazz;
private:
    static const int VMObjectNumberOfFields;
};



#endif
