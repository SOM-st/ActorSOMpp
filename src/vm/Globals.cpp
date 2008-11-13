#include "Globals.h"

#include "../vmobjects/VMObject.h"
#include "../vmobjects/VMClass.h"
#include "Universe.h"

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

void Globals::InitializeGlobals()
{
    nilObject = new (_HEAP) VMObject;
    nilObject->SetField(0, nilObject);
    
    cout << "We have a Nil object" << endl;
    /*VMArray* vmo = new (heap, 4*sizeof(VMObject*)) VMArray(4);
    VMMethod*/
    metaClassClass = _UNIVERSE->NewMetaclassClass();
    cout << "Metaclass Class created, creating System classes" << endl;
    objectClass    = _UNIVERSE->NewSystemClass();
    nilClass       = _UNIVERSE->NewSystemClass();
    classClass     = _UNIVERSE->NewSystemClass();
    arrayClass     = _UNIVERSE->NewSystemClass();
    symbolClass    = _UNIVERSE->NewSystemClass();
    methodClass    = _UNIVERSE->NewSystemClass();
    integerClass   = _UNIVERSE->NewSystemClass();
    bigIntegerClass= _UNIVERSE->NewSystemClass();
    frameClass     = _UNIVERSE->NewSystemClass();
    primitiveClass = _UNIVERSE->NewSystemClass();
    stringClass    = _UNIVERSE->NewSystemClass();
    doubleClass    = _UNIVERSE->NewSystemClass();
    cout << "System classes created" << endl;
    
    nilObject->SetClass(nilClass);

    cout << "Initialize System Classes" << endl;
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

    cout << "System classes initialized, now let's load them!"
            << endl;

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

    cout << "YAY, the system classes are loaded" << endl;
    
    cout << "loading block class" << endl;
    blockClass = _UNIVERSE->LoadClass(_UNIVERSE->SymbolForChars("Block"));
    
    cout << "setting up true and false" << endl;
    trueObject = _UNIVERSE->NewInstance(_UNIVERSE->LoadClass(_UNIVERSE->SymbolForChars("True")));
    falseObject = _UNIVERSE->NewInstance(_UNIVERSE->LoadClass(_UNIVERSE->SymbolForChars("False")));

    cout << "load System" << endl;
    systemClass = _UNIVERSE->LoadClass(_UNIVERSE->SymbolForChars("System"));
}



VMObject* Globals::NilObject()
{
    return nilObject;
}

VMObject* Globals::TrueObject()
{
    return trueObject;
}

VMObject* Globals::FalseObject()
{
    return falseObject;
}

VMClass* Globals::ObjectClass()
{
    return objectClass;
}

VMClass* Globals::ClassClass()
{
    return classClass;
}

VMClass* Globals::MetaClassClass()
{
    return metaClassClass;
}

VMClass* Globals::NilClass()
{
    return nilClass;
}

VMClass* Globals::IntegerClass()
{
    return integerClass;
}

VMClass* Globals::BigIntegerClass()
{
    return bigIntegerClass;
}

VMClass* Globals::ArrayClass()
{
    return arrayClass;
}

VMClass* Globals::MethodClass()
{
    return methodClass;
}

VMClass* Globals::SymbolClass()
{
    return symbolClass;
}

VMClass* Globals::FrameClass()
{
    return frameClass;
}

VMClass* Globals::PrimitiveClass()
{
    return primitiveClass;
}

VMClass* Globals::StringClass()
{
    return stringClass;
}

VMClass* Globals::SystemClass()
{
    return systemClass;
}

VMClass* Globals::BlockClass()
{
    return blockClass;
}

VMClass* Globals::DoubleClass()
{
    return doubleClass;
}

