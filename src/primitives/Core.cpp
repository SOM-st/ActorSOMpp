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
#include "../primitivesCore/Routine.h"
#include "String.h"
#include "Symbol.h"
#include "System.h"
#include "../primitivesCore/Primitive.h"
#include "../primitivesCore/PrimitiveLoader.h"

static PrimitiveLoader* loader = NULL;
//map<pString, Primitive*> primitiveObjects;
//"Constructor"
static bool initialized = false;
extern "C" void setup() {
    if (!loader) {
        //Initialize loader
        loader = new PrimitiveLoader();
        loader->AddPrimitiveObject("Array", 
            static_cast<Primitive*>(new _Array()));

        loader->AddPrimitiveObject("BigInteger", 
            static_cast<Primitive*>(new _BigInteger()));

        loader->AddPrimitiveObject("Block", 
            static_cast<Primitive*>(new _Block()));

        loader->AddPrimitiveObject("Class", 
            static_cast<Primitive*>(new _Class()));

        loader->AddPrimitiveObject("Double", 
            static_cast<Primitive*>(new _Double()));

        loader->AddPrimitiveObject("Integer", 
            static_cast<Primitive*>(new _Integer()));

        loader->AddPrimitiveObject("Object", 
            static_cast<Primitive*>(new _Object()));

        loader->AddPrimitiveObject("String", 
            static_cast<Primitive*>(new _String()));

        loader->AddPrimitiveObject("Symbol", 
            static_cast<Primitive*>(new _Symbol()));

        loader->AddPrimitiveObject("System", 
            static_cast<Primitive*>(new _System()));
        
        
    }
}

extern "C" bool supports_class(const char* name) {
    if (!loader) setup();
    return loader->SupportsClass(name);
}



extern "C" void tearDown() {
    //primitiveClasses.clear();
    if (loader) delete loader;
    //if (primitiveObjects) delete primitiveObjects;
}

extern "C" PrimitiveRoutine* create(const pString& cname, const pString& fname) {

#ifdef __DEBUG
    cout << "Loading Primitive: " << cname << "::" << fname << endl;
#endif
    if (!loader) setup();
    
    return loader->GetPrimitiveRoutine(cname, fname);
    

}

