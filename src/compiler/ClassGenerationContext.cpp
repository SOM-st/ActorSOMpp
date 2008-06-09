#include "ClassGenerationContext.h"


class_generation_context::class_generation_context() {
	/*name = NULL;
	super_name = NULL;*/
    class_side = false;
    /*instance_fields = List_new();
    instance_methods = List_new();
    class_fields = List_new();
    class_methods = List_new();*/
}

class_generation_context::~class_generation_context()
{
}

void class_generation_context::add_class_field(pString field) {
	this->class_fields.push_back(field);
}
void class_generation_context::add_instance_field(pString field) {
	this->instance_fields.push_back(field);
}