#include <string.h>

//#include "../misc/defs.h"
//#include "../vmobjects/VMObject.h"
#include "../vmobjects/PrimitiveRoutine.h"

#include "Array.h"
#include "BigInteger.h"
#include "Block.h"
#include "Class.h"
#include "Double.h"
#include "Integer.h"
#include "Object.h"
#include "String.h"
#include "Symbol.h"
#include "System.h"

#include "../primitivesCore/Routine.h"
#include "../primitivesCore/PrimitiveContainer.h"
#include "../primitivesCore/PrimitiveLoader.h"

static PrimitiveLoader* loader = NULL;
//map<StdString, PrimitiveContainer*> primitiveObjects;
//"Constructor"
static bool initialized = false;
#if defined(_MSC_VER)
#include "Core.h"

Universe* universe;
Heap* heap;
VMObject* trueObject;
VMObject* falseObject;
VMObject* nilObject;


extern "C" void setup(Universe* uni, Heap* h, VMObject** globals) {
    universe = uni;
    heap = h;
    trueObject = globals[0];
    falseObject = globals[1];
    nilObject = globals[2];
#else
extern "C" void setup() {
#endif
    if (!loader) {
        //Initialize loader
        loader = new PrimitiveLoader();
        loader->AddPrimitiveObject("Array", 
            static_cast<PrimitiveContainer*>(new _Array()));

        loader->AddPrimitiveObject("BigInteger", 
            static_cast<PrimitiveContainer*>(new _BigInteger()));

        loader->AddPrimitiveObject("Block", 
            static_cast<PrimitiveContainer*>(new _Block()));

        loader->AddPrimitiveObject("Class", 
            static_cast<PrimitiveContainer*>(new _Class()));

        loader->AddPrimitiveObject("Double", 
            static_cast<PrimitiveContainer*>(new _Double()));

        loader->AddPrimitiveObject("Integer", 
            static_cast<PrimitiveContainer*>(new _Integer()));

        loader->AddPrimitiveObject("Object", 
            static_cast<PrimitiveContainer*>(new _Object()));

        loader->AddPrimitiveObject("String", 
            static_cast<PrimitiveContainer*>(new _String()));

        loader->AddPrimitiveObject("Symbol", 
            static_cast<PrimitiveContainer*>(new _Symbol()));

        loader->AddPrimitiveObject("System", 
            static_cast<PrimitiveContainer*>(new _System()));
    }
}

extern "C" bool supportsClass(const char* name) {
#if defined (__GNUC__)
    if (!loader) setup();
#endif
    return loader->SupportsClass(name);
}



extern "C" void tearDown() {
    //primitiveClasses.clear();
    if (loader) delete loader;
    //if (primitiveObjects) delete primitiveObjects;
}

extern "C" PrimitiveRoutine* create(const StdString& cname, const StdString& fname) {

#ifdef __DEBUG
    cout << "Loading PrimitiveContainer: " << cname << "::" << fname << endl;
#endif
#if defined (__GNUC__)
    if (!loader) setup();
#endif

    return loader->GetPrimitiveRoutine(cname, fname);
}

