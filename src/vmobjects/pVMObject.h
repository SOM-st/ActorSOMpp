#pragma once

#ifndef PVMOBJECT_H_
#define PVMOBJECT_H_
class VMObject;

class pVMObject
{
public:
    pVMObject() { pointer = NULL; };
    ~pVMObject() { /*nothing to do*/ };

    VMObject* operator ->()
    {
        return pointer;
    }

    pVMObject& operator =(VMObject* ptr)
    {
        int32_t val = (int32_t) ptr;
        if ((val & 0x80000000) != 0)  //highes bit set?
        {
            val = val & 0x7FFFFFFF;
        }
        pointer = (VMObject*)val;
        return (*this);
    }
private:
    VMObject* pointer;

};

#endif