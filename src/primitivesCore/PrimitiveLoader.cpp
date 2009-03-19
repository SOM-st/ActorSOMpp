#include "PrimitiveLoader.h"
#include "PrimitiveContainer.h"

#include <vmobjects/PrimitiveRoutine.h>


PrimitiveLoader::PrimitiveLoader() {
    primitiveObjects =  map<StdString, PrimitiveContainer*>();
}

PrimitiveLoader::~PrimitiveLoader() {
    map<StdString, PrimitiveContainer*>::iterator it = primitiveObjects.begin();
    for (; it != primitiveObjects.end(); ++it) {
        delete it->second;
    }
   
}

void PrimitiveLoader::AddPrimitiveObject( const char* name, PrimitiveContainer* prim) {
    primitiveObjects[StdString(name)] = prim;
}

bool PrimitiveLoader::SupportsClass( const char* name ) {
    return primitiveObjects[StdString(name)] != NULL;
}

PrimitiveRoutine* PrimitiveLoader::GetPrimitiveRoutine( const std::string& cname, const std::string& mname ) {
    PrimitiveRoutine* result; 
    PrimitiveContainer* primitive = primitiveObjects[cname];
    if (!primitive) {
        cout << "Primitive object not found for name: " << cname << endl;
        return NULL;
    }
    result = primitive->GetPrimitive(mname);
    if (!result) {
        cout << "method " << mname << " not found in class" << cname << endl;
        return NULL;
    }
    return result;
}

