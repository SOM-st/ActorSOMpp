#include "PrimitiveContainer.h"

PrimitiveContainer::PrimitiveContainer() {
    methods = new map<StdString, PrimitiveRoutine*>();
}

PrimitiveContainer::~PrimitiveContainer()
{
    delete methods;
}

void PrimitiveContainer::SetPrimitive( const char* name, PrimitiveRoutine* routine )
{
    (*methods)[StdString(name)] = routine;
}

PrimitiveRoutine* PrimitiveContainer::GetPrimitive(const StdString& routineName) {
    return (*methods)[routineName];
}
