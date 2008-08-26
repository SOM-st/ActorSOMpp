#include "VMClass.h"


bool VMClass::add_instance_invokable(VMObject *ptr)
{
	return 0;
}

void VMClass::add_instance_primitive(VMPrimitive *ptr)
{

}

VMSymbol* VMClass::get_instance_field_name(int)
{
	return NULL;
}

VMClass* VMClass::get_super_class()
{
	return super_class;
}

void VMClass::set_super_class(VMClass* sup)
{
	super_class = sup;
}

VMSymbol* VMClass::get_name()
{
	return name;
}
void VMClass::set_name(VMSymbol* nam)
{
	name = nam;
}

VMArray* VMClass::get_instance_fields()
{
	return instance_fields;
}


void VMClass::set_instance_fields(VMArray* inst_fields)
{
	instance_fields = inst_fields;
}

VMArray  *VMClass::get_instance_invokables()
{
	return instance_invokables;
}

void      VMClass::set_instance_invokables(VMArray* invokables)
{
	instance_invokables = invokables;
}

int       VMClass::get_number_of_instance_invokables()
{
	//return instance_invokables.size();
	return 0;
}

VMObject *VMClass::get_instance_invokable(int index)
{
	//return instance_invokables[index];
	return NULL;
}

void      VMClass::set_instance_invokable(int index, VMObject* invokable)
{
	//instance_invokables[index] = invokable;
}

VMObject* VMClass::lookup_invokable(VMSymbol* name)
{
	return NULL;
}

int       VMClass::lookup_field_index(VMSymbol* name)
{
	return 0;
}


int       VMClass::get_number_of_instance_fields()
{
	//return instance_fields.size();
	return 0;
}

bool      VMClass::has_primitives()
{
	return 0;
}

void      VMClass::load_primitives(const pString* name,int i)
{
}
