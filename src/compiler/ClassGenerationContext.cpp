#include "ClassGenerationContext.h"
#include "../vmobjects/VMSymbol.h"
#include "../vmobjects/VMObject.h"
#include "../vmobjects/VMClass.h"

class_generation_context::class_generation_context() {
	name = NULL;
	super_name = NULL;
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

VMClass* class_generation_context::Assemble()
{
    // build class class name
    pString ccname = string(name->GetStdString()) + " class";
    
    // Load the super class
    VMClass* super_class = _UNIVERSE->load_class(super_name);
    
    // Allocate the class of the resulting class
    VMClass* result_class = _UNIVERSE->new_class(metaclass_class);

    // Initialize the class of the resulting class
    result_class->set_instance_fields(_UNIVERSE->new_array_list(class_fields));
    result_class->set_instance_invokables(_UNIVERSE->new_array_list(class_methods));
    result_class->set_name(_UNIVERSE->symbol_for(ccname));

    VMClass* super_mclass = super_class->GetClass();
    result_class->set_super_class(super_mclass);
    
    // Allocate the resulting class
    VMClass* result = _UNIVERSE->new_class(result_class);
    
    // Initialize the resulting class
    result->set_instance_fields(_UNIVERSE->new_array_list(instance_fields));
    result->set_instance_invokables(_UNIVERSE->new_array_list(instance_methods));
    result->set_name(_UNIVERSE->symbol_for(ccname));
    result->set_super_class(super_class);
    
    return result;
}

void class_generation_context::AssembleSystemClass( VMClass* system_class )
{
    system_class->set_instance_invokables(_UNIVERSE->new_array_list(instance_methods));
    system_class->set_instance_fields(_UNIVERSE->new_array_list(instance_fields));
    // class-bound == class-instance-bound 
    VMClass* super_mclass = system_class->GetClass();
    super_mclass->set_instance_invokables(_UNIVERSE->new_array_list(class_methods));
    super_mclass->set_instance_fields(_UNIVERSE->new_array_list(class_fields));
}


