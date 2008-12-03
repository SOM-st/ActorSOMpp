#pragma once

#ifndef PVMOBJECT_H_
#define PVMOBJECT_H_
#include "VMObject.h";
#include "../vm/Universe.h"
#include <typeinfo>

template<class T = VMObject>
class pVMObject
{
public:
    pVMObject<T>() { pointer = NULL; };
    pVMObject<T>(T* o) { pointer = o; if (!IsTaggedInteger()) pointer->IncreaseGCCount(); };
    pVMObject<T>(pVMObject<T>& o) { (pointer = o.pointer)->IncreaseGCCount(); };
    ~pVMObject<T>() { if (pointer) pointer->DecreaseGCCount(); };
    
    template<class U>
    pVMObject<T>(pVMObject<U>& o) { 
        pointer = dynamic_cast<T*>((VMObject*)o);
        if (!pointer) {
            cout << "Trying to convert unmatching types: " << typeid(U).name()
                 << " to " << typeid(T).name() << endl;
            throw std::bad_exception();
        }
    };

    bool IsTaggedInteger();
    operator int32_t();
    //operator pVMObject<VMObject>&();
    operator VMObject*() { return pointer; };
    //operator T*() { return pointer; };

    T& operator*() {
        if (IsTaggedInteger()) {
            cout << "Dereferencing Tagged Integer" << endl;
            throw std::bad_exception();
        }
        return *pointer;
    }

    T* operator ->() {
        if (IsTaggedInteger()) {
            //return SomeGlobalIntegerObjectThatAnswersMethodCallsForAllTaggedIntegers;
        }
        return pointer;
    }

    pVMObject<T>& operator =(T* ptr);
    pVMObject<T>& operator =(T& ptr);
    pVMObject<T>& operator =(pVMObject<T>& ptr);
    //pVMObject<T>& operator =(pVMObject<VMObject>& ptr);
    pVMObject<T>& operator =(int32_t value);
    
    template<class U>
    pVMObject<T>& operator =(pVMObject<U>& o){ 
        pointer = dynamic_cast<T*>((VMObject*)o);
        if (!pointer) {
            //warn for now, but later nothing should happen as this might be wanted
            //to check for NULL
            cout << "Warning: Trying to convert unmatching types: " 
                 << typeid(U).name() << " to " << typeid(T).name() << endl;
            //throw std::bad_exception();
        }
    };

private:
    
    void SetIntegerValue(int32_t value);
    T* pointer;

};



#include "VMInteger.h"
#include "VMBigInteger.h"

template < class T >
pVMObject<T>& pVMObject<T>::operator =( T& ptr )
{
    if (pointer) pointer->DecreaseGCCount();
    (pointer = &ptr)->IncreaseGCCount();
    return *this;
}

template < class T >
pVMObject<T>& pVMObject<T>::operator =( T* ptr )
{
    if (pointer) pointer->DecreaseGCCount();
    (pointer = ptr)->IncreaseGCCount();
    return *this;
}

template <class T>
pVMObject<T>& pVMObject<T>::operator =(pVMObject<T>& ptr)
{
    if (pointer) pointer->DecreaseGCCount();
    (pointer = ptr.pointer)->IncreaseGCCount();
    return *this;
}

template <class T>
pVMObject<T>& pVMObject<T>::operator =(int32_t value)
{
    if (pointer) pointer->DecreaseGCCount();
    this->SetIntegerValue(value);
    return *this;
}

template <class T>
bool pVMObject<T>::IsTaggedInteger() {
    return (((int32_t)pointer & 0x80000000) != 0);
}

template <class T>
pVMObject<T>::operator int32_t() { 
    if (IsTaggedInteger())  //highes bit set?
    {
        return ((int32_t)pointer & 0x7FFFFFFF);
    }
    VMInteger* vmi;
    if ((vmi = dynamic_cast<T*>(pointer)) == NULL) {
        cout << "Bad cast to integer from non (tagged) integer VMObject." << endl;
        throw std::bad_typeid();
    }
    return vmi->GetEmbeddedInteger();
    
};

template <class T>
void pVMObject<T>::SetIntegerValue( int32_t value )
{
    // Check with tagged integer border
    if(value & 0x80000000) {
        //it's too big, so we need a VMBigInteger
        pointer = static_cast<T*>(_UNIVERSE->NewInteger(value));
        pointer->IncreaseGCCount();
    }
    else 
        pointer = (T*) (value | 0x80000000);
}



#endif


