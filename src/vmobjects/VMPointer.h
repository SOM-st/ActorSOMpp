#pragma once

#ifndef PVMOBJECT_H_
#define PVMOBJECT_H_

#include <typeinfo>
#include <iostream>

#include "../misc/defs.h"

#include "../memory/ObjectTable.h"

class VMInteger;
extern VMInteger* integerProxy;

template<class T>
class VMPointer {
public:
    //Constructors
    
    //Default constructor creates NULL pointer
    VMPointer<T>() {
        index.index.value = 0;
        index.index.is_iint = false;
    };
    
    // construct an immediate signed integer
    explicit VMPointer<T>(int32_t val) {
        //index.index.value = val;
        index.index.is_iint = true;
        SetEmbeddedInteger(val);
    }
    
    VMPointer<T>(ObjectTable::Index i) { 
        index.index = i;
    };
    
    // add object to the object table
    VMPointer<T>(VMObject* o) { 
        index.index = _OBJECT_TABLE.AddObject(o);
    };
    
    VMPointer<T>(const VMPointer<T>& o) { 
        index.index = o.index.index;
    };
    
    //Type conversion constructor. Converts pointer to class U in pointer to class T
    template<class U>
    VMPointer<T>(VMPointer<U>& o) { 
        index.index = o.index.index;
    };
    
    template<class U>
    VMPointer<T>(const VMPointer<U>& o) { 
        index.index = o.index.index;
    };
    
    //Type conversion with type checking. Important for VMPointerConverter.
    template<class U>
    VMPointer<T>(VMPointer<U>& o, const bool check_cast) {
        if (check_cast) {
            // This sucks.
            T* ptr_to;
            if (NULL == (ptr_to = dynamic_cast<T*>(
                                    o.index.index.is_iint ? (U*) integerProxy :
                                        (U*) _OBJECT_TABLE[o.index.index]))) {
                index.index.value = 0; // make it a NULL pointer
                index.index.is_iint = false;
                return;
            }
        }
        index.index = o.index.index;
    };
    
    //null checking
    inline bool IsNull() const {
        return !index.index.is_iint && (void*) _OBJECT_TABLE[index.index] == NULL; 
    };
    
    inline bool IsImmediateInteger() const {
        return index.index.is_iint;
    }

    //type conversions

    //conversion to "dumb pointer" of the included type
    inline operator T*() {
        return (T*) _OBJECT_TABLE[index.index]; 
    };

    //conversion to reference of the included type
    inline T& operator*() {
        return *((T*) _OBJECT_TABLE[index.index]);
    };

    //conversion to const reference of the included type
    inline T& operator*() const {
        return *((T*) _OBJECT_TABLE[index.index]);
    };
    
    inline ObjectTable::Index GetIndex() {
        return index.index;
    }

    //member access operator
    inline T* operator ->() {
        if (!index.index.is_iint) {
            return (T*) _OBJECT_TABLE[index.index];
        }
        return (T*) integerProxy;
    };
    
    //member access operator for const methods
    inline T* operator ->() const {
        if (!index.index.is_iint) {
            return (T*) _OBJECT_TABLE[index.index];
        }
        return (T*) integerProxy;
    };
    
    
    void SetEmbeddedInteger(int32_t val);
    int32_t GetEmbeddedInteger() const;
    
    //assignments
    
    //allows to assign smart pointers of the same type
    inline VMPointer<T>& operator =(VMPointer<T> ptr);
    
    //comparing
    inline bool operator==(VMPointer<T> o) const{
        return o.index.index.value == index.index.value;
    };
    
    template <class U>
    inline bool operator==(VMPointer<U> o) const{
        return index.index.value == o.index.index.value;
    };
    
    inline bool operator!=(VMPointer<T> o) const{
        return o.index.index.value != index.index.value;
    };
        
    template <class U>
    inline bool operator!=(VMPointer<U> o) const{
        return index.index.value != o.index.index.value;
    };

    //neccessary to use VMPointers as keys in std::map
    inline int operator<(VMPointer<T> o) const {
        return index.index.value < o.index.index.value;
    }
    
    // remove this pointer from the object table, 
    // converts it to a NULL-pointer
    inline void unlink() {
        _OBJECT_TABLE.RemoveObject(index.index);
        index.index.value = 0;
    }
    
    //inline bool IsLocalObject() {
    //
    //}

protected:
    // We need this because object table indexes are unsigned
    typedef union {
        ObjectTable::Index index;
        struct immediate_int_value {
            int32_t   value:31;
            bool    is_iint: 1;
        } iint;
    } Index;
    
    // Index of the referenced object in the object table.
    Index index;
    
    template<class> friend class VMPointer;
};

template<class T>
VMPointer<T>& VMPointer<T>::operator=(VMPointer<T> ptr)
{
    if(&ptr == this) return *this;
    index = ptr.index;
    return *this;
};

#endif
