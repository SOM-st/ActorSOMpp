#include "Globals.h"
#include "Universe.h"

#include "../vmobjects/VMObject.h"
#include "../vmobjects/VMClass.h"

//
//the static global objects
//
pVMObject Globals::nilObject;
pVMObject Globals::trueObject;
pVMObject Globals::falseObject;
      
pVMClass Globals::objectClass;
pVMClass Globals::classClass;
pVMClass Globals::metaClassClass;
  
pVMClass Globals::nilClass;
pVMClass Globals::integerClass;
pVMClass Globals::bigIntegerClass;
pVMClass Globals::arrayClass;
pVMClass Globals::methodClass;
pVMClass Globals::symbolClass;
pVMClass Globals::frameClass;
pVMClass Globals::primitiveClass;
pVMClass Globals::stringClass;
pVMClass Globals::systemClass;
pVMClass Globals::blockClass;
pVMClass Globals::doubleClass;

///
//Call this method before using any of the globals or it will crash!
//
void Globals::InitializeGlobals() {
    //
    //allocate nil object
    //
    nilObject = new (_HEAP) VMObject;
    nilObject->SetField(0, nilObject);

    metaClassClass = _UNIVERSE->NewMetaclassClass();

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
    
    nilObject->SetClass(nilClass);

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

    blockClass = _UNIVERSE->LoadClass(_UNIVERSE->SymbolForChars("Block"));

    trueObject = _UNIVERSE->NewInstance(_UNIVERSE->LoadClass(_UNIVERSE->SymbolForChars("True")));
    falseObject = _UNIVERSE->NewInstance(_UNIVERSE->LoadClass(_UNIVERSE->SymbolForChars("False")));

    systemClass = _UNIVERSE->LoadClass(_UNIVERSE->SymbolForChars("System"));
}

//Singleton access methods

pVMObject Globals::NilObject() {
    return nilObject;
}

pVMObject Globals::TrueObject() {
    return trueObject;
}

pVMObject Globals::FalseObject() {
    return falseObject;
}

pVMClass Globals::ObjectClass() {
    return objectClass;
}

pVMClass Globals::ClassClass() {
    return classClass;
}

pVMClass Globals::MetaClassClass() {
    return metaClassClass;
}

pVMClass Globals::NilClass() {
    return nilClass;
}

pVMClass Globals::IntegerClass() {
    return integerClass;
}

pVMClass Globals::BigIntegerClass() {
    return bigIntegerClass;
}

pVMClass Globals::ArrayClass() {
    return arrayClass;
}

pVMClass Globals::MethodClass() {
    return methodClass;
}

pVMClass Globals::SymbolClass() {
    return symbolClass;
}

pVMClass Globals::FrameClass() {
    return frameClass;
}

pVMClass Globals::PrimitiveClass() {
    return primitiveClass;
}

pVMClass Globals::StringClass() {
    return stringClass;
}

pVMClass Globals::SystemClass() {
    return systemClass;
}

pVMClass Globals::BlockClass() {
    return blockClass;
}

pVMClass Globals::DoubleClass() {
    return doubleClass;
}

