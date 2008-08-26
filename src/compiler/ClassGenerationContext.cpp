#include "ClassGenerationContext.h"


class_generation_context::class_generation_context() {
	/*name = NULL;
	super_name = NULL;*/
    class_side = false;
	instance_fields.clear();
    instance_methods.clear();
	class_fields.clear();
    class_methods.clear();
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
bool class_generation_context::find_field(pString field) {

	pList fields = is_class_side() ?
        class_fields :
        instance_fields;
    return index_of(fields, field) != -1;//fields.index_of(field);//SEND(fields, indexOf, Universe_symbol_for(field)) != -1;

}

void class_generation_context::add_instance_method(pString method) {
	this->instance_methods.push_back(method);
}

void class_generation_context::add_class_method(pString method) {
	this->class_methods.push_back(method);
}
