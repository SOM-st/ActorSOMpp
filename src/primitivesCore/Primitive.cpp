#include "Primitive.h"

Primitive* PrimitiveObject = NULL;

Primitive::Primitive() {
    methods = new map<pString, PrimitiveRoutine*>();
}

Primitive::~Primitive()
{
    delete methods;
}

void Primitive::SetRoutine( const char* name, PrimitiveRoutine* routine )
{
    (*methods)[pString(name)] = routine;
}

PrimitiveRoutine* Primitive::GetRoutine(const pString& routineName) {
    return (*methods)[routineName];
}
