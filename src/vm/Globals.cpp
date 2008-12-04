#include "Globals.h"

#include "../vmobjects/VMObject.h"
#include "../vmobjects/VMClass.h"
#include "Universe.h"

//
//the static global objects
//
VMObject* Globals::nilObject;
VMObject* Globals::trueObject;
VMObject* Globals::falseObject;
      
VMClass* Globals::objectClass;
VMClass* Globals::classClass;
VMClass* Globals::metaClassClass;
  
VMClass* Globals::nilClass;
VMClass* Globals::integerClass;
VMClass* Globals::bigIntegerClass;
VMClass* Globals::arrayClass;
VMClass* Globals::methodClass;
VMClass* Globals::symbolClass;
VMClass* Globals::frameClass;
VMClass* Globals::primitiveClass;
VMClass* Globals::stringClass;
VMClass* Globals::systemClass;
VMClass* Globals::blockClass;
VMClass* Globals::doubleClass;
//#define __DEBUG
///
//Call this method before using any of the globals or it will crash!
//
void Globals::InitializeGlobals() {
    //
    //allocate nil object
    //
    nilObject = new (_HEAP) VMObject;
    nilObject->SetField(0, nilObject);
    
#ifdef __DEBUG
    cout << "We have a Nil object" << endl;
#endif __DEBUG

    metaClassClass = _UNIVERSE->NewMetaclassClass();

#ifdef __DEBUG
    cout << "Metaclass Class created, creating System classes" << endl;
#endif __DEBUG

    objectClass     = _UNIVERSE->NewSystemClass();
    nilClass        = _UNIVERSE->NewSystemClass();
    classClass      = _UNIVERSE->NewSystemClass();
    arrayClass      = _UNIVERSE->NewSystemClass();
    symbolClass     = _UNIVERSE->NewSystemClass();
    methodClass     = _UNIVERSE->NewSystemClass();
    integerClass    = _UNIVERSE->NewSystemClass();
    bigIntegerClass = _UNIVERSE->NewSystemClass();
    frameClass      = _UNIVERSE->NewSystemClass();
    primitiveClass  = _UNIVERSE->NewSystemClass();
    stringClass     = _UNIVERSE->NewSystemClass();
    doubleClass     = _UNIVERSE->NewSystemClass();

#ifdef __DEBUG
    cout << "System classes created" << endl;
#endif __DEBUG
    
    nilObject->SetClass(nilClass);

#ifdef __DEBUG
    cout << "Initialize System Classes" << endl;
#endif __DEBUG

    _UNIVERSE->InitializeSystemClass(objectClass, NULL, "Object");
    _UNIVERSE->InitializeSystemClass(classClass, objectClass, "Class");
    _UNIVERSE->InitializeSystemClass(metaClassClass, classClass, "Metaclass");
    _UNIVERSE->InitializeSystemClass(nilClass, objectClass, "Nil");
    _UNIVERSE->InitializeSystemClass(arrayClass, objectClass, "Array");
    _UNIVERSE->InitializeSystemClass(methodClass, arrayClass, "Method");
    _UNIVERSE->InitializeSystemClass(symbolClass, objectClass, "Symbol");
    _UNIVERSE->InitializeSystemClass(integerClass, objectClass, "Integer");
    _UNIVERSE->InitializeSystemClass(bigIntegerClass, objectClass,
                                     "BigInteger");
    _UNIVERSE->InitializeSystemClass(frameClass, arrayClass, "Frame");
    _UNIVERSE->InitializeSystemClass(primitiveClass, objectClass,
                                     "Primitive");
    _UNIVERSE->InitializeSystemClass(stringClass, objectClass, "String");
    _UNIVERSE->InitializeSystemClass(doubleClass, objectClass, "Double");

#ifdef __DEBUG
    cout << "System classes initialized, now let's load them!"
            << endl;
#endif __DEBUG

    _UNIVERSE->LoadSystemClass(objectClass);
    _UNIVERSE->LoadSystemClass(classClass);
    _UNIVERSE->LoadSystemClass(metaClassClass);
    _UNIVERSE->LoadSystemClass(nilClass);
    _UNIVERSE->LoadSystemClass(arrayClass);
    _UNIVERSE->LoadSystemClass(methodClass);
    _UNIVERSE->LoadSystemClass(symbolClass);
    _UNIVERSE->LoadSystemClass(integerClass);
    _UNIVERSE->LoadSystemClass(bigIntegerClass);
    _UNIVERSE->LoadSystemClass(frameClass);
    _UNIVERSE->LoadSystemClass(primitiveClass);
    _UNIVERSE->LoadSystemClass(stringClass);
    _UNIVERSE->LoadSystemClass(doubleClass);

#ifdef __DEBUG
    cout << "YAY, the system classes are loaded" << endl;
    cout << "loading block class" << endl;
#endif __DEBUG

    blockClass = _UNIVERSE->LoadClass(_UNIVERSE->SymbolForChars("Block"));

#ifdef __DEBUG
    cout << "setting up true and false" << endl;
#endif __DEBUG

    trueObject = _UNIVERSE->NewInstance(_UNIVERSE->LoadClass(_UNIVERSE->SymbolForChars("True")));
    falseObject = _UNIVERSE->NewInstance(_UNIVERSE->LoadClass(_UNIVERSE->SymbolForChars("False")));

#ifdef __DEBUG
    cout << "load System" << endl;
#endif __DEBUG

    systemClass = _UNIVERSE->LoadClass(_UNIVERSE->SymbolForChars("System"));
}



VMObject* Globals::NilObject() {
    return nilObject;
}

VMObject* Globals::TrueObject() {
    return trueObject;
}

VMObject* Globals::FalseObject() {
    return falseObject;
}

VMClass* Globals::ObjectClass() {
    return objectClass;
}

VMClass* Globals::ClassClass() {
    return classClass;
}

VMClass* Globals::MetaClassClass() {
    return metaClassClass;
}

VMClass* Globals::NilClass() {
    return nilClass;
}

VMClass* Globals::IntegerClass() {
    return integerClass;
}

VMClass* Globals::BigIntegerClass() {
    return bigIntegerClass;
}

VMClass* Globals::ArrayClass() {
    return arrayClass;
}

VMClass* Globals::MethodClass() {
    return methodClass;
}

VMClass* Globals::SymbolClass() {
    return symbolClass;
}

VMClass* Globals::FrameClass() {
    return frameClass;
}

VMClass* Globals::PrimitiveClass() {
    return primitiveClass;
}

VMClass* Globals::StringClass() {
    return stringClass;
}

VMClass* Globals::SystemClass() {
    return systemClass;
}

VMClass* Globals::BlockClass() {
    return blockClass;
}

VMClass* Globals::DoubleClass() {
    return doubleClass;
}

