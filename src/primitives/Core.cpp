///*
// * $Id: Core.c 229 2008-04-22 14:10:50Z tobias.pape $
// *
//Copyright (c) 2007 Michael Haupt, Tobias Pape
//Software Architecture Group, Hasso Plattner Institute, Potsdam, Germany
//http://www.hpi.uni-potsdam.de/swa/
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in
//all copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//THE SOFTWARE.
//  */
//
//#include <stdlib.h>
//#include <stdio.h>
//#include <stdbool.h>
//#include <string.h>
//
//#include "Array.h"
//#include "BigInteger.h"
//#include "Block.h"
//#include "Class.h"
//#include "Double.h"
//#include "Integer.h"
//#include "Object.h"
//#include "String.h"
//#include "Symbol.h"
//#include "System.h"
//
///* Lib initialization */
//#pragma mark * Load-time execution *
///**
// * Both the following functions have to do with load/unload-time execution.
// * That is, anything in init/_init is being executed upon every loading of the
// * library, even if this very library is _not_ responsible for the class in
// * question.
// *
// * Use with care.
// */
//
//// Library load initializer function declaration. 
//// This is compiler/os-specific
//#ifdef __GNUC__
//void init(void) __attribute__((constructor));
//void fini(void) __attribute__((destructor));
//#elif
//void _init(void);
//void _fini(void);
//#pragma init _init
//#pragma fini _fini
//#endif __GNUC__
//
//// Library load initializer
//#ifdef __GNUC__
//void init(void) { ; /* noop */}
//#elif
//void _init(void) { ; /* noop */ }
//#endif __GNUC__
//
//// Library unload function
//#ifdef __GNUC__
//void fini(void) { ; /* noop */ }
//#elif
//void _fini(void) { ; /* noop */ }
//#endif __GNUC__
//
//
//// Classes supported by this lib.
//static char* supported_classes[] = {
//    "Array",
//    "BigInteger",
//    "Block", //this is Block1..3
//    "Class",
//    "Double",
//    "Integer",
//    "Object",
//    "String",
//    "Symbol",
//    "System",
//    NULL
//};
//
//
///*************************************************/
//#pragma mark * Exported functions starting here  *
///*************************************************/
//
//// returns, whether this lib is responsible for a specific class
//bool supports_class(const char* name) {
//    char **iter=supported_classes;
//    while(*iter)
//        if(strcmp(name, *iter++)==0)
//            return true;
//    return false;
//}
//
//
///**
// * init_csp()
// *
// * the library initializer. It is not equal to the init/fini-pair, as for them, 
// * the init is executed upon every loading of the shared library.
// *
// * All work that needs to be done before the actual primitives are assigned
// * should be called from this function.
// */
//void init_csp(void) {
//    // Call init funcions.
//    __System_init();
//    __Integer_init();
//}
//


//#include "../misc/defs.h"
#include "../vmobjects/VMObject.h"
#include "../vmobjects/PrimitiveRoutine.h"
#include "../vm/Universe.h"

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

map<pString, Primitive*> primitiveClasses;

void Core::setup()
{
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
    /*primitiveClasses[pString("Array")] = (Primitive*) Array;
    primitiveClasses[pString("BigInteger")] = (Primitive*) BigInteger;
    primitiveClasses[pString("Block")] = (Primitive*) Block;
    primitiveClasses[pString("Class")] = (Primitive*) Class;
    primitiveClasses[pString("Double")] = (Primitive*) Double;
    primitiveClasses[pString("Integer")] = (Primitive*) Integer;
    primitiveClasses[pString("Object")] = (Primitive*) Object;
    primitiveClasses[pString("String")] = (Primitive*) String;
    primitiveClasses[pString("Symbol")] = (Primitive*) Symbol;
    primitiveClasses[pString("System")] = (Primitive*) System;*/
}

void Core::tearDown()
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

PrimitiveRoutine* Core::create(const pString& cname, const pString& fname)
{
    PrimitiveRoutine* result;

    //Primitive* primClass = primitiveClasses[cname];

    if (cname == "Array") {
        if (fname == "new")
            result = new Routine<_Array>(Array, &_Array::New);
        else if (fname == "at")
            result = new Routine<_Array>(Array, &_Array::At);
        else if (fname == "at_put")
            result = new Routine<_Array>(Array, &_Array::AtPut);
        else if (fname == "length")
            result = new Routine<_Array>(Array, &_Array::Length);
        cout << "method " << fname << "not found in class" << cname << endl;
        return NULL;
    } else  if (cname == "BigInteger") {
        if (fname == "plus")
            result = new Routine<_BigInteger>(BigInteger, &_BigInteger::Plus);
        else if (fname == "minus")
            result = new Routine<_BigInteger>(BigInteger, &_BigInteger::Minus);
        else if (fname == "star")
            result = new Routine<_BigInteger>(BigInteger, &_BigInteger::Star);
        else if (fname == "slash")
            result = new Routine<_BigInteger>(BigInteger, &_BigInteger::Slash);
        else if (fname == "percent")
            result = new Routine<_BigInteger>(BigInteger, &_BigInteger::Percent);
        else if (fname == "and")
            result = new Routine<_BigInteger>(BigInteger, &_BigInteger::And);
        else if (fname == "equal")
            result = new Routine<_BigInteger>(BigInteger, &_BigInteger::Equal);
        else if (fname == "lowerthan")
            result = new Routine<_BigInteger>(BigInteger, &_BigInteger::Lowerthan);
        else if (fname == "asString")
            result = new Routine<_BigInteger>(BigInteger, &_BigInteger::AsString);
        else if (fname == "sqrt")
            result = new Routine<_BigInteger>(BigInteger, &_BigInteger::Sqrt);
        cout << "method " << fname << "not found in class" << cname << endl;
        return NULL;
    } else  if (cname == "Block") {
        if (fname == "value")
            result = new Routine<_Block>(Block, &_Block::Value);
        else if (fname == "restart")
            result = new Routine<_Block>(Block, &_Block::Restart);
        else if (fname == "value_")
            result = new Routine<_Block>(Block, &_Block::Value_);
        else if (fname == "value_with_")
            result = new Routine<_Block>(Block, &_Block::Value_with_);
        cout << "method " << fname << "not found in class" << cname << endl;
        return NULL;
    } else  if (cname == "Class") {
        if (fname == "new")
            result = new Routine<_Class>(Class, &_Class::New);
        cout << "method " << fname << "not found in class" << cname << endl;
        return NULL;
    } else  if (cname == "Double") {
        if (fname == "plus")
            result = new Routine<_Double>(Double, &_Double::Plus);
        else if (fname == "minus")
            result = new Routine<_Double>(Double, &_Double::Minus);
        else if (fname == "star")
            result = new Routine<_Double>(Double, &_Double::Star);
        else if (fname == "slashslash")
            result = new Routine<_Double>(Double, &_Double::Slashslash);
        else if (fname == "percent")
            result = new Routine<_Double>(Double, &_Double::Percent);
        else if (fname == "and")
            result = new Routine<_Double>(Double, &_Double::And);
        else if (fname == "equal")
            result = new Routine<_Double>(Double, &_Double::Equal);
        else if (fname == "lowerthan")
            result = new Routine<_Double>(Double, &_Double::Lowerthan);
        else if (fname == "asString")
            result = new Routine<_Double>(Double, &_Double::AsString);
        else if (fname == "sqrt")
            result = new Routine<_Double>(Double, &_Double::Sqrt);
        cout << "method " << fname << "not found in class" << cname << endl;
        return NULL;
    } else  if (cname == "Integer") {
        if (fname == "plus")
            result = new Routine<_Integer>(Integer, &_Integer::Plus);
        else if (fname == "minus")
            result = new Routine<_Integer>(Integer, &_Integer::Minus);
        else if (fname == "star")
            result = new Routine<_Integer>(Integer, &_Integer::Star);
        else if (fname == "slash")
            result = new Routine<_Integer>(Integer, &_Integer::Slash);
        else if (fname == "slashslash")
            result = new Routine<_Integer>(Integer, &_Integer::Slashslash);
        else if (fname == "percent")
            result = new Routine<_Integer>(Integer, &_Integer::Percent);
        else if (fname == "and")
            result = new Routine<_Integer>(Integer, &_Integer::And);
        else if (fname == "equal")
            result = new Routine<_Integer>(Integer, &_Integer::Equal);
        else if (fname == "lowerthan")
            result = new Routine<_Integer>(Integer, &_Integer::Lowerthan);
        else if (fname == "asString")
            result = new Routine<_Integer>(Integer, &_Integer::AsString);
        else if (fname == "sqrt")
            result = new Routine<_Integer>(Integer, &_Integer::Sqrt);
        cout << "method " << fname << "not found in class" << cname << endl;
        return NULL;
    } else  if (cname == "Object") {
        if (fname == "equalequal")
            result = new Routine<_Object>(Object, &_Object::Equalequal);
        else if (fname == "objectSize")
            result = new Routine<_Object>(Object, &_Object::ObjectSize);
        else if (fname == "hashcode")
            result = new Routine<_Object>(Object, &_Object::Hashcode);
        cout << "method " << fname << "not found in class" << cname << endl;
        return NULL;
    } else  if (cname == "String") {
        if (fname == "concatenate")
            result = new Routine<_String>(String, &_String::Concatenate_);
        else if (fname == "asSymbol")
            result = new Routine<_String>(String, &_String::AsSymbol);
        else if (fname == "hashcode")
            result = new Routine<_String>(String, &_String::Hashcode);
        else if (fname == "length")
            result = new Routine<_String>(String, &_String::Length);
        else if (fname == "equal")
            result = new Routine<_String>(String, &_String::Equal);
        else if (fname == "primSubstringFrom_To_")
            result = new Routine<_String>(String, &_String::PrimSubstringFrom_To_);
        cout << "method " << fname << "not found in class" << cname << endl;
        return NULL;
    } else  if (cname == "Symbol") {
        if (fname == "asString")
            result = new Routine<_Symbol>(Symbol, &_Symbol::AsString);
        cout << "method " << fname << "not found" << endl;
        return NULL;
    } else  if (cname == "System") {
        if (fname == "global")
            result = new Routine<_System>(System_, &_System::Global_);
        else if (fname == "global_put_")
            result = new Routine<_System>(System_, &_System::Global_put_);
        else if (fname == "load_")
            result = new Routine<_System>(System_, &_System::Load_);
        else if (fname == "exit_")
            result = new Routine<_System>(System_, &_System::Exit_);
        else if (fname == "printString_")
            result = new Routine<_System>(System_, &_System::PrintString_);
        else if (fname == "printNewline")
            result = new Routine<_System>(System_, &_System::PrintNewline);
        else if (fname == "time")
            result = new Routine<_System>(System_, &_System::Time);
        cout << "method " << fname << "not found in class" << cname << endl;
        return NULL;
    } else {
        cout << "Primitive Class not found" << endl;
        return NULL;
    }
    return result;
}

void Core::destroy(PrimitiveRoutine* rtn)
{
    delete(rtn);
}
