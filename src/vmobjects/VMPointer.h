#pragma once

#ifndef PVMOBJECT_H_
#define PVMOBJECT_H_

#include <typeinfo>
#include <iostream>

#include "../misc/defs.h"

#include "../memory/ObjectTable.h"

template<class T>
class VMPointer {
public:
    
    //Constructors
    
    //Default constructor creates NULL pointer
    VMPointer<T>() : index(0) { 
    };
    
    VMPointer<T>(ObjectTable::Index index) : index(index) { 
    };
    
    // Access to this should be retricted as it can introduce duplicates into the object table.
    VMPointer<T>(VMObject* o) { 
        index = _OBJECT_TABLE.AddObject(o);
    };
    
    VMPointer<T>(const VMPointer<T>& o) { 
        index = o.GetIndex();
    };
    
    //Type conversion constructor. Converts pointer to class U in pointer to class T
    template<class U>
    VMPointer<T>(VMPointer<U>& o) { 
        index = o.GetIndex();
    };
    
    template<class U>
    VMPointer<T>(const VMPointer<U>& o) { 
        index = o.GetIndex();
    };
    
    //Type conversion with type checking. Important for VMPointerConverter.
    template<class U>
    VMPointer<T>(VMPointer<U>& o, const bool check_cast) {
        if (check_cast) {
            // This sucks.
            T* ptr_to;
            if (NULL == (ptr_to = dynamic_cast<T*>((U*) _OBJECT_TABLE[o.GetIndex()]))) {
                index = 0; // make it a NULL pointer
                return;
            }
        }
        index = o.GetIndex();
    };
    
    //null checking
    inline bool IsNull() const {
        return (void*) _OBJECT_TABLE[index] == NULL; 
    };

    //type conversions

    //conversion to "dumb pointer" of the included type
    inline operator T*() { 
        return (T*) _OBJECT_TABLE[index]; 
    };

    //conversion to reference of the included type
    inline T& operator*() {
        return *((T*) _OBJECT_TABLE[index]);
    };

    //conversion to const reference of the included type
    inline T& operator*() const {
        return *((T*) _OBJECT_TABLE[index]);
    };

    //member access operator
    inline T* operator ->() {
        T* addr = (T*) _OBJECT_TABLE[index];
        if (NULL == addr) {
            std::cout << "[VMPointer] dereferencing null-pointer" << std::endl;
        }
        return addr;
    };
    
    //member access operator for const methods
    inline T* operator ->() const{
        T* addr = (T*) _OBJECT_TABLE[index];
        if (NULL == addr) {
            std::cout << "[VMPointer] dereferencing null-pointer" << std::endl;
        }
        return addr;
    };
    
    //assignments
    
    //allows to assign smart pointers of the same type
    inline VMPointer<T>& operator =(VMPointer<T> ptr);
    
    //comparing
    inline bool operator==(VMPointer<T> o) const{
        return o.index == index;
    };
    
    template <class U>
    inline bool operator==(VMPointer<U> o) const{
        return index == o.GetIndex();
    };
    
    inline bool operator!=(VMPointer<T> o) const{
        return o.GetIndex() != index;
    };
        
    template <class U>
    inline bool operator!=(VMPointer<U> o) const{
        return index != o.GetIndex();
    };

    //neccessary to use VMPointers as keys in std::map
    inline int operator<(VMPointer<T> o) const {
        return index < o.GetIndex();
    }

    inline ObjectTable::Index GetIndex() const{
        return index;
    }

protected:
    
    // Index of the referenced object in the object table.
    ObjectTable::Index index;
    
};


template < class T >
VMPointer<T>& VMPointer<T>::operator =(VMPointer<T> ptr)
{
    if(&ptr == this) return *this;
    index = ptr.index;
    return *this;
};

#endif
