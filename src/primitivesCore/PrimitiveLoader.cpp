#include "PrimitiveLoader.h"
#include "Primitive.h"
#include "../vmobjects/PrimitiveRoutine.h"


PrimitiveLoader::PrimitiveLoader() {
    primitiveObjects =  map<pString, Primitive*>();
}

PrimitiveLoader::~PrimitiveLoader() {
    map<pString, Primitive*>::iterator it = primitiveObjects.begin();
    for (; it != primitiveObjects.end(); ++it) {
        delete it->second;
    }
   
}

void PrimitiveLoader::AddPrimitiveObject( const char* name, Primitive* prim) {
    primitiveObjects[pString(name)] = prim;
}

bool PrimitiveLoader::SupportsClass( const char* name ) {
    return primitiveObjects[pString(name)] != NULL;
}

PrimitiveRoutine* PrimitiveLoader::GetPrimitiveRoutine( const pString& cname, pString mname ) {
    PrimitiveRoutine* result; 
    Primitive* primitive = primitiveObjects[cname];
    if (!primitive) {
        cout << "Primitive object not found for name: " << cname << endl;
        return NULL;
    }
    result = primitive->GetRoutine(mname);
    if (!result) {
        cout << "method " << mname << " not found in class" << cname << endl;
        return NULL;
    }
    return result;
}

