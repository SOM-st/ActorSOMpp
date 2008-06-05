#pragma once

#include "../misc/defs.h"

class ClassGenerationContext
{
	pVMSymbol name;
    pVMSymbol super_name;
    bool      class_side;
    list<string>     instance_fields;
    list<string>     instance_methods;
    list<string>     class_fields;
    list<string>     class_methods;
public:
	ClassGenerationContext();
	~ClassGenerationContext();
};