
//#include "../misc/defs.h"
#include "../vmobjects/VMObject.h"
#include "../vmobjects/PrimitiveRoutine.h"
#include "../vm/Universe.h"
#include "../misc/defs.h"
#include "Array.h"
#include "BigInteger.h"
#include "Block.h"
#include "Class.h"
#include "Double.h"
#include "Integer.h"
#include "Object.h"
#include "Routine.h"
#include "String.h"
#include "Symbol.h"
#include "System.h"

//map<pString, Primitive*> primitiveClasses;
// Classes supported by this lib.
static char* supported_classes[] = {
    "Array",
    "BigInteger",
    "Block", //this is Block1..3
    "Class",
    "Double",
    "Integer",
    "Object",
    "String",
    "Symbol",
    "System",
    NULL
};

static bool initialized = false;

extern "C" bool supports_class(const char* name) {
    char **iter=supported_classes;
    while(*iter)
        if(strcmp(name, *iter++)==0)
            return true;
    return false;
}

extern "C" void setup()
{
    if (!initialized) {
        //primitiveClasses.clear();
        Array = new _Array();
        BigInteger = new _BigInteger();
        Block = new _Block();
        Class = new _Class();
        Double = new _Double();
        Integer = new _Integer();
        Object = new _Object();
        String = new _String();
        Symbol = new _Symbol();
        System_ = new _System();
        initialized = true;
    }
    /*primitiveClasses[pString("Array"))] = (Primitive*) Array;
    primitiveClasses[pString("BigInteger"))] = (Primitive*) BigInteger;
    primitiveClasses[pString("Block"))] = (Primitive*) Block;
    primitiveClasses[pString("Class"))] = (Primitive*) Class;
    primitiveClasses[pString("Double"))] = (Primitive*) Double;
    primitiveClasses[pString("Integer"))] = (Primitive*) Integer;
    primitiveClasses[pString("Object"))] = (Primitive*) Object;
    primitiveClasses[pString("String"))] = (Primitive*) String;
    primitiveClasses[pString("Symbol"))] = (Primitive*) Symbol;
    primitiveClasses[pString("System"))] = (Primitive*) System;*/
}

extern "C" void tearDown()
{
    //primitiveClasses.clear();
    delete(Array);
    delete(BigInteger);
    delete(Block);
    delete(Class);
    delete(Double);
    delete(Integer);
    delete(Object);
    delete(String);
    delete(Symbol);
    delete(System_);
}

extern "C" PrimitiveRoutine* create(const pString& cname, const pString& fname)
{
    PrimitiveRoutine* result;
    
    //Primitive* primClass = primitiveClasses[cname];

    if (cname == pString("Array")) {
        if (fname == pString("new_"))
            result = new (_HEAP) Routine<_Array>(Array, &_Array::New_);
        else if (fname == pString("at_"))
            result = new (_HEAP) Routine<_Array>(Array, &_Array::At_);
        else if (fname == pString("at_put_"))
            result = new (_HEAP) Routine<_Array>(Array, &_Array::AtPut_);
        else if (fname == pString("length"))
            result = new (_HEAP) Routine<_Array>(Array, &_Array::Length);
        else {
            cout << "method " << fname << " not found in class" << cname << endl;
            return NULL;
        }
    } else  if (cname == pString("BigInteger")) {
        if (fname == pString("plus"))
            result = new (_HEAP) Routine<_BigInteger>(BigInteger, &_BigInteger::Plus);
        else if (fname == pString("minus"))
            result = new (_HEAP) Routine<_BigInteger>(BigInteger, &_BigInteger::Minus);
        else if (fname == pString("star"))
            result = new (_HEAP) Routine<_BigInteger>(BigInteger, &_BigInteger::Star);
        else if (fname == pString("slash"))
            result = new (_HEAP) Routine<_BigInteger>(BigInteger, &_BigInteger::Slash);
        else if (fname == pString("percent"))
            result = new (_HEAP) Routine<_BigInteger>(BigInteger, &_BigInteger::Percent);
        else if (fname == pString("and"))
            result = new (_HEAP) Routine<_BigInteger>(BigInteger, &_BigInteger::And);
        else if (fname == pString("equal"))
            result = new (_HEAP) Routine<_BigInteger>(BigInteger, &_BigInteger::Equal);
        else if (fname == pString("lowerthan"))
            result = new (_HEAP) Routine<_BigInteger>(BigInteger, &_BigInteger::Lowerthan);
        else if (fname == pString("asString"))
            result = new (_HEAP) Routine<_BigInteger>(BigInteger, &_BigInteger::AsString);
        else if (fname == pString("sqrt"))
            result = new (_HEAP) Routine<_BigInteger>(BigInteger, &_BigInteger::Sqrt);
        else {
            cout << "method " << fname << "not found in class" << cname << endl;
            return NULL;
        }
    } else  if (cname == pString("Block")) {
        if (fname == pString("value"))
            result = new (_HEAP) Routine<_Block>(Block, &_Block::Value);
        else if (fname == pString("restart"))
            result = new (_HEAP) Routine<_Block>(Block, &_Block::Restart);
        else if (fname == pString("value_"))
            result = new (_HEAP) Routine<_Block>(Block, &_Block::Value_);
        else if (fname == pString("value_with_"))
            result = new (_HEAP) Routine<_Block>(Block, &_Block::Value_with_);
        else {
            cout << "method " << fname << "not found in class" << cname << endl;
            return NULL;
        }
    } else  if (cname == pString("Class")) {
        if (fname == pString("new"))
            result = new (_HEAP) Routine<_Class>(Class, &_Class::New);
        else {
            cout << "method " << fname << "not found in class" << cname << endl;
            return NULL;
        }
    } else  if (cname == pString("Double")) {
        if (fname == pString("plus"))
            result = new (_HEAP) Routine<_Double>(Double, &_Double::Plus);
        else if (fname == pString("minus"))
            result = new (_HEAP) Routine<_Double>(Double, &_Double::Minus);
        else if (fname == pString("star"))
            result = new (_HEAP) Routine<_Double>(Double, &_Double::Star);
        else if (fname == pString("slashslash"))
            result = new (_HEAP) Routine<_Double>(Double, &_Double::Slashslash);
        else if (fname == pString("percent"))
            result = new (_HEAP) Routine<_Double>(Double, &_Double::Percent);
        else if (fname == pString("and"))
            result = new (_HEAP) Routine<_Double>(Double, &_Double::And);
        else if (fname == pString("equal"))
            result = new (_HEAP) Routine<_Double>(Double, &_Double::Equal);
        else if (fname == pString("lowerthan"))
            result = new (_HEAP) Routine<_Double>(Double, &_Double::Lowerthan);
        else if (fname == pString("asString"))
            result = new (_HEAP) Routine<_Double>(Double, &_Double::AsString);
        else if (fname == pString("sqrt"))
            result = new (_HEAP) Routine<_Double>(Double, &_Double::Sqrt);
        else {
            cout << "method " << fname << "not found in class" << cname << endl;
            return NULL;
        }
    } else  if (cname == pString("Integer")) {
        if (fname == pString("plus"))
            result = new (_HEAP) Routine<_Integer>(Integer, &_Integer::Plus);
        else if (fname == pString("minus"))
            result = new (_HEAP) Routine<_Integer>(Integer, &_Integer::Minus);
        else if (fname == pString("star"))
            result = new (_HEAP) Routine<_Integer>(Integer, &_Integer::Star);
        else if (fname == pString("slash"))
            result = new (_HEAP) Routine<_Integer>(Integer, &_Integer::Slash);
        else if (fname == pString("slashslash"))
            result = new (_HEAP) Routine<_Integer>(Integer, &_Integer::Slashslash);
        else if (fname == pString("percent"))
            result = new (_HEAP) Routine<_Integer>(Integer, &_Integer::Percent);
        else if (fname == pString("and"))
            result = new (_HEAP) Routine<_Integer>(Integer, &_Integer::And);
        else if (fname == pString("equal"))
            result = new (_HEAP) Routine<_Integer>(Integer, &_Integer::Equal);
        else if (fname == pString("lowerthan"))
            result = new (_HEAP) Routine<_Integer>(Integer, &_Integer::Lowerthan);
        else if (fname == pString("asString"))
            result = new (_HEAP) Routine<_Integer>(Integer, &_Integer::AsString);
        else if (fname == pString("sqrt"))
            result = new (_HEAP) Routine<_Integer>(Integer, &_Integer::Sqrt);
        else if (fname == pString("atRandom"))
            result = new (_HEAP) Routine<_Integer>(Integer, &_Integer::AtRandom);
        else {
            cout << "method " << fname << "not found in class" << cname << endl;
            return NULL;
        }
    } else  if (cname == pString("Object")) {
        if (fname == pString("equalequal"))
            result = new (_HEAP) Routine<_Object>(Object, &_Object::Equalequal);
        else if (fname == pString("objectSize"))
            result = new (_HEAP) Routine<_Object>(Object, &_Object::ObjectSize);
        else if (fname == pString("hashcode"))
            result = new (_HEAP) Routine<_Object>(Object, &_Object::Hashcode);
        else {
            cout << "method " << fname << " not found in class" << cname << endl;
            return NULL;
        }
    } else  if (cname == pString("String")) {
        if (fname == pString("concatenate_"))
            result = new (_HEAP) Routine<_String>(String, &_String::Concatenate_);
        else if (fname == pString("asSymbol"))
            result = new (_HEAP) Routine<_String>(String, &_String::AsSymbol);
        else if (fname == pString("hashcode"))
            result = new (_HEAP) Routine<_String>(String, &_String::Hashcode);
        else if (fname == pString("length"))
            result = new (_HEAP) Routine<_String>(String, &_String::Length);
        else if (fname == pString("equal"))
            result = new (_HEAP) Routine<_String>(String, &_String::Equal);
        else if (fname == pString("primSubstringFrom_To_"))
            result = new (_HEAP) Routine<_String>(String, &_String::PrimSubstringFrom_To_);
        else {
            cout << "method " << fname << "not found in class" << cname << endl;
            return NULL;
        }
    } else  if (cname == pString("Symbol")) {
        if (fname == pString("asString"))
            result = new (_HEAP) Routine<_Symbol>(Symbol, &_Symbol::AsString);
       else {
            cout << "method " << fname << "not found in class" << cname << endl;
            return NULL;
        }
    } else  if (cname == pString("System")) {
        if (fname == pString("global_"))
            result = new (_HEAP) Routine<_System>(System_, &_System::Global_);
        else if (fname == pString("global_put_"))
            result = new (_HEAP) Routine<_System>(System_, &_System::Global_put_);
        else if (fname == pString("load_"))
            result = new (_HEAP) Routine<_System>(System_, &_System::Load_);
        else if (fname == pString("exit_"))
            result = new (_HEAP) Routine<_System>(System_, &_System::Exit_);
        else if (fname == pString("printString_"))
            result = new (_HEAP) Routine<_System>(System_, &_System::PrintString_);
        else if (fname == pString("printNewline"))
            result = new (_HEAP) Routine<_System>(System_, &_System::PrintNewline);
        else if (fname == pString("time"))
            result = new (_HEAP) Routine<_System>(System_, &_System::Time);
        else {
            cout << "method " << fname << "not found in class" << cname << endl;
            return NULL;
        }
    } else {
        cout << "Primitive Class not found" << endl;
        return NULL;
    }
#ifdef __DEBUG
    cout << "Primitive " << cname << "::" << fname << " Loaded" << endl;
#endif
    return result;
}

extern "C" void destroy(PrimitiveRoutine* rtn)
{
    delete(rtn, _HEAP);
}
