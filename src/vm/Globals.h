#pragma once

#ifndef GLOBALS_H_
#define GLOBALS_H_

class VMObject;
class VMClass;

#if defined(__GNUC__)
#define DLL_EXPORT
#else
#define DLL_EXPORT __declspec(dllexport)
#endif
#include "../vmobjects/ObjectFormats.h"
class DLL_EXPORT Globals {
public:
    static void InitializeGlobals();

    static pVMObject NilObject();//Globals::NilObject();
    static pVMObject TrueObject();//Globals::TrueObject();
    static pVMObject FalseObject();//Globals::FalseObject();
      
    static pVMClass ObjectClass();//Globals::ObjectClass();
    static pVMClass ClassClass();//Globals::ClassClass();
    static pVMClass MetaClassClass();//Globals::MetaClassClass();
  
    static pVMClass NilClass();//Globals::NilClass();
    static pVMClass IntegerClass();//Globals::IntegerClass();
    static pVMClass BigIntegerClass();//Globals::BigIntegerClass();
    static pVMClass ArrayClass();//Globals::ArrayClass();
    static pVMClass MethodClass();//Globals::MethodClass();
    static pVMClass SymbolClass();//Globals::SymbolClass();
    static pVMClass FrameClass();//Globals::FrameClass();
    static pVMClass PrimitiveClass();//Globals::PrimitiveClass()();
    static pVMClass StringClass();//Globals::StringClass();
    static pVMClass SystemClass();//Globals::SystemClass();
    static pVMClass BlockClass();//Globals::BlockClass();
    static pVMClass DoubleClass();//Globals::DoubleClass();

private:
    
    static pVMObject nilObject;
    static pVMObject trueObject;
    static pVMObject falseObject;
      
    static pVMClass objectClass;
    static pVMClass classClass;
    static pVMClass metaClassClass;
  
    static pVMClass nilClass;
    static pVMClass integerClass;
    static pVMClass bigIntegerClass;
    static pVMClass arrayClass;
    static pVMClass methodClass;
    static pVMClass symbolClass;
    static pVMClass frameClass;
    static pVMClass primitiveClass;
    static pVMClass stringClass;
    static pVMClass systemClass;
    static pVMClass blockClass;
    static pVMClass doubleClass;
};

#endif GLOBALS_H_
