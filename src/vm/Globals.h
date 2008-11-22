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

class DLL_EXPORT Globals {
public:
    static void InitializeGlobals();

    static VMObject* NilObject();//Globals::NilObject();
    static VMObject* TrueObject();//Globals::TrueObject();
    static VMObject* FalseObject();//Globals::FalseObject();
      
    static VMClass* ObjectClass();//Globals::ObjectClass();
    static VMClass* ClassClass();//Globals::ClassClass();
    static VMClass* MetaClassClass();//Globals::MetaClassClass();
  
    static VMClass* NilClass();//Globals::NilClass();
    static VMClass* IntegerClass();//Globals::IntegerClass();
    static VMClass* BigIntegerClass();//Globals::BigIntegerClass();
    static VMClass* ArrayClass();//Globals::ArrayClass();
    static VMClass* MethodClass();//Globals::MethodClass();
    static VMClass* SymbolClass();//Globals::SymbolClass();
    static VMClass* FrameClass();//Globals::FrameClass();
    static VMClass* PrimitiveClass();//Globals::PrimitiveClass()();
    static VMClass* StringClass();//Globals::StringClass();
    static VMClass* SystemClass();//Globals::SystemClass();
    static VMClass* BlockClass();//Globals::BlockClass();
    static VMClass* DoubleClass();//Globals::DoubleClass();

private:
    
    static VMObject* nilObject;
    static VMObject* trueObject;
    static VMObject* falseObject;
      
    static VMClass* objectClass;
    static VMClass* classClass;
    static VMClass* metaClassClass;
  
    static VMClass* nilClass;
    static VMClass* integerClass;
    static VMClass* bigIntegerClass;
    static VMClass* arrayClass;
    static VMClass* methodClass;
    static VMClass* symbolClass;
    static VMClass* frameClass;
    static VMClass* primitiveClass;
    static VMClass* stringClass;
    static VMClass* systemClass;
    static VMClass* blockClass;
    static VMClass* doubleClass;
};

#endif GLOBALS_H_
