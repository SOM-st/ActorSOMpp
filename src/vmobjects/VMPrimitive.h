#pragma once

#ifndef VMPRIMITVE_H_
#define VMPRIMITVE_H_

#include "VMObject.h"
#include "PrimitiveRoutine.h"
#include "VMInvokable.h"

//#include "../compiler/MethodGenerationContext.h"

class VMSymbol;

class VMPrimitive : public VMInvokable, public VMObject {
public:
    static pVMPrimitive GetEmptyPrimitive(pVMSymbol sig);

    VMPrimitive(pVMSymbol sig);
    //virtual ~VMPrimitive();
    
    virtual inline bool    IsEmpty() const;
    virtual inline void    SetRoutine(PrimitiveRoutine* rtn);
    virtual void    MarkReferences();
    virtual void    SetEmpty(bool value) { empty = (bool*)value; };

    //-----------VMInvokable-------//
    //operator "()" to invoke the primitive
    virtual void    operator()(pVMFrame frm) { (*routine)(this, frm); };
    virtual pVMSymbol GetSignature() const;
	virtual void      SetSignature(pVMSymbol sig);
	virtual pVMClass  GetHolder() const;
	virtual void      SetHolder(pVMClass hld);
    virtual bool      IsPrimitive() const { return true; };
    

    /**
     * Needs to be implemented for VMPrimitive, so setting the object size works
     */
	void *operator new( size_t num_bytes, Heap *heap, 
                        unsigned int additional_bytes = 0) {
        /*if (num_bytes == 24) {
            cout << "hier";
        }
        cout << "Allocating " << num_bytes << "+" << additional_bytes << " = " << num_bytes + additional_bytes << "Bytes" <<endl;*/
        size_t rSize;
        void* mem = (void*)heap->AllocateObject(num_bytes + additional_bytes, &rSize);
        pVMPrimitive tis = (pVMPrimitive)mem;
        tis->objectSize = rSize;
        return mem;
	}

    void operator delete(void* self, Heap *heap, 
                         unsigned int /*additional_bytes*/) {
        int size = ((pVMObject)self)->GetObjectSize();
		heap->Free(self, size);
	}

	 void operator delete( void *self, Heap *heap) {
         int size = ((pVMObject)self)->GetObjectSize();
		 heap->Free(self, size); 
	 } 
private:
    void EmptyRoutine(pVMObject self, pVMFrame frame);

    pVMSymbol signature;
    pVMClass holder;

    PrimitiveRoutine* routine;
    bool* empty;

    static const int VMPrimitiveNumberOfFields;
    
};

bool VMPrimitive::IsEmpty() const {
    return (bool)empty;
}


void VMPrimitive::SetRoutine(PrimitiveRoutine* rtn) {
    routine = rtn;
}

#endif
