#include <string.h>

//#include "../misc/defs.h"
//#include "../vmobjects/VMObject.h"
#include "../vmobjects/PrimitiveRoutine.h"
#include "../vm/Universe.h"
#include "../memory/Heap.h"
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
#include "Primitive.h"

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

Universe* universe;
Heap* heap;

extern "C" bool supports_class(const char* name) {
    char **iter=supported_classes;
    while(*iter)
        if(strcmp(name, *iter++)==0)
            return true;
    return false;
}

map<pString, Primitive*> primitiveObjects;

extern "C" void setup(Universe* uni, Heap* h)
{
    if (!initialized) {
        primitiveObjects = map<pString, Primitive*>();
        universe = uni;
        heap = h;
        
        //Create Primitive objects
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

        //Add them to the map
        primitiveObjects[pString("Array")] = static_cast<Primitive*>(Array);
        primitiveObjects[pString("BigInteger")] = static_cast<Primitive*>(BigInteger);
        primitiveObjects[pString("Block")] = static_cast<Primitive*>(Block);
        primitiveObjects[pString("Class")] = static_cast<Primitive*>(Class);
        primitiveObjects[pString("Double")] = static_cast<Primitive*>(Double);
        primitiveObjects[pString("Integer")] = static_cast<Primitive*>(Integer);
        primitiveObjects[pString("Object")] = static_cast<Primitive*>(Object);
        primitiveObjects[pString("String")] = static_cast<Primitive*>(String);
        primitiveObjects[pString("Symbol")] = static_cast<Primitive*>(Symbol);
        primitiveObjects[pString("System")] = static_cast<Primitive*>(System_);
        initialized = true;
    }
    
    
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
    Primitive* primitive = primitiveObjects[cname];
    if (!primitive) {
        cout << "Primitive object not found for name: " << cname << endl;
        return NULL;
    }
    result = primitive->GetRoutine(fname);
    if (!result) {
        cout << "method " << fname << " not found in class" << cname << endl;
        return NULL;
    }
    /*if (cname == pString("Array")) {
        
    } else  if (cname == pString("BigInteger")) {
        
    } else  if (cname == pString("Block")) {
        
    } else  if (cname == pString("Class")) {
        
    } else  if (cname == pString("Double")) {
        
    } else  if (cname == pString("Integer")) {
        
    } else  if (cname == pString("Object")) {
        
    } else  if (cname == pString("String")) {
        
    } else  if (cname == pString("Symbol")) {
        
    } else  if (cname == pString("System")) {
        
    } else {
        cout << "Primitive Class not found" << endl;
        return NULL;
    }*/
#ifdef __DEBUG
    cout << "Primitive " << cname << "::" << fname << " Loaded" << endl;
#endif
    return result;
}

extern "C" void destroy(PrimitiveRoutine* rtn)
{
    delete(rtn, heap);
}
