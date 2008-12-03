#pragma once

#ifndef PVMOBJECT_H_
#define PVMOBJECT_H_

#include <typeinfo>

#include "../vm/Universe.h"

#include "VMObject.h"


template<class T = VMObject>
class VMPointer {
public:
    VMPointer<T>() { pointer = NULL; };
    VMPointer<T>(T* o) { pointer = o; if (!IsTaggedInteger()) pointer->IncreaseGCCount(); };
    VMPointer<T>(VMPointer<T>& o) { (pointer = o.pointer)->IncreaseGCCount(); };
    ~VMPointer<T>() { if (pointer) pointer->DecreaseGCCount(); };
    
    template<class U>
    VMPointer<T>(VMPointer<U>& o) { 
        pointer = dynamic_cast<T*>((VMObject*)o);
        if (!pointer) {
            cout << "Trying to convert unmatching types: " << typeid(U).name()
                 << " to " << typeid(T).name() << endl;
            throw std::bad_exception();
        }
    };

    bool IsTaggedInteger();
    bool IsNull() { return pointer == NULL; } ;

    operator int32_t();
    //operator VMPointer<VMObject>&();

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

    VMPointer<T>& operator =(T* ptr);
    VMPointer<T>& operator =(T& ptr);
    VMPointer<T>& operator =(VMPointer<T>& ptr);
    //VMPointer<T>& operator =(VMPointer<VMObject>& ptr);
    VMPointer<T>& operator =(int32_t value);
    
    template<class U>
    VMPointer<T>& operator =(VMPointer<U>& o){ 
        pointer = dynamic_cast<T*>((VMObject*)o);
        if (!pointer) {
            //warn for now, but later nothing should happen as this might be wanted
            //to check for NULL
            cout << "Warning: Trying to convert unmatching types: " 
                 << typeid(U).name() << " to " << typeid(T).name() << endl;
            //throw std::bad_exception();
        }
    };

    bool operator==(VMPointer<T>& o) {
        return o.pointer == pointer;
    }

    template <class U>
    bool operator==(VMPointer<U>& o) {
        return pointer == (VMObject*)o;
    }
    
    bool operator!=(VMPointer<T>& o) {
        return o.pointer != pointer;
    }

    template <class U>
    bool operator!=(VMPointer<U>& o) {
        return pointer != (VMObject*)o;
    }

private:
    
    void SetIntegerValue(int32_t value);
    T* pointer;

};



#include "VMInteger.h"
#include "VMBigInteger.h"

template < class T >
VMPointer<T>& VMPointer<T>::operator =( T& ptr )
{
    if (pointer) pointer->DecreaseGCCount();
    (pointer = &ptr)->IncreaseGCCount();
    return *this;
}

template < class T >
VMPointer<T>& VMPointer<T>::operator =( T* ptr )
{
    if (pointer) pointer->DecreaseGCCount();
    (pointer = ptr)->IncreaseGCCount();
    return *this;
}

template <class T>
VMPointer<T>& VMPointer<T>::operator =(VMPointer<T>& ptr)
{
    if (pointer) pointer->DecreaseGCCount();
    (pointer = ptr.pointer)->IncreaseGCCount();
    return *this;
}

template <class T>
VMPointer<T>& VMPointer<T>::operator =(int32_t value)
{
    if (pointer) pointer->DecreaseGCCount();
    this->SetIntegerValue(value);
    return *this;
}

template <class T>
bool VMPointer<T>::IsTaggedInteger() {
    return (((int32_t)pointer & 0x80000000) != 0);
}

template <class T>
VMPointer<T>::operator int32_t() { 
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
void VMPointer<T>::SetIntegerValue( int32_t value )
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


