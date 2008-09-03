#include "ClassGenerationContext.h"
#include "../vmobjects/VMSymbol.h"
#include "../vmobjects/VMObject.h"

class_generation_context::class_generation_context() {
	/*name = NULL;
	super_name = NULL;*/
    class_side = false;
	instance_fields.Clear();
    instance_methods.Clear();
	class_fields.Clear();
    class_methods.Clear();
}

class_generation_context::~class_generation_context()
{
}

void class_generation_context::add_class_field(VMObject* field) {
	this->class_fields.Add(field);
}
void class_generation_context::add_instance_field(VMObject* field) {
	this->instance_fields.Add(field);
}
bool class_generation_context::find_field(const pString& field) {

	pList fields = is_class_side() ?
        class_fields :
        instance_fields;
    return fields.IndexOf( (VMObject*)_UNIVERSE->symbol_for(field)) != -1;//fields.IndexOf(field);//SEND(fields, IndexOf, Universe_symbol_for(field)) != -1;

}

void class_generation_context::add_instance_method(VMObject* method) {
	this->instance_methods.Add(method);
}

void class_generation_context::add_class_method(VMObject* method) {
	this->class_methods.Add(method);
}

