#include "VMClass.h"
#include "VMArray.h"
#include "VMSymbol.h"

bool VMClass::add_instance_invokable(VMObject *ptr)
{
	for (int i = 0; i < instance_invokables->GetNumberOfIndexableFields(); ++i)
	{
		VMObject* cmp = instance_invokables->GetItem(i);
		if (cmp != 0) {
			//if (ptr->GetSignature() == ((VMInvokable*)cmp)->GetSignature()) {
			//	  instance_invokables->AddItem(i, ptr);
			//	  return false;
			//}
		}
	}
	if (instance_invokables->GetNumberOfIndexableFields() >= instance_invokables->GetArraySize())
		instance_invokables = instance_invokables->CopyAndExtendWith(ptr);
	else instance_invokables->AddItem(ptr);

	return true;
}

void VMClass::add_instance_primitive(VMPrimitive *ptr)
{
	if (add_instance_invokable((VMObject*)ptr)) {
		//cout << "Warn: Primitive "<<ptr->GetSignature<<" is not in class definition for class " << name->GetStdString() << endl;
	}
}

VMSymbol* VMClass::get_instance_field_name(int index)
{
	if (index >= numberOfSuperInstanceFields())
	{
		index -= numberOfSuperInstanceFields();
		return (VMSymbol*) instance_fields->GetIndexableField(index);
	}
	
	return super_class->get_instance_field_name(index);
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
	return instance_invokables->GetNumberOfIndexableFields();
}

VMObject *VMClass::get_instance_invokable(int index)
{
	return instance_invokables->GetItem(index);
	//return instance_invokables[index];
	//return NULL;
}

void      VMClass::set_instance_invokable(int index, VMObject* invokable)
{
	instance_invokables->AddItem(index, invokable);
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
	return instance_fields->GetNumberOfIndexableFields();
}

bool      VMClass::has_primitives()
{
	return 0;
}

void      VMClass::load_primitives(const pString* name,int i)
{
}


void VMClass::MarkReferences()
{
	VMObject::MarkReferences();
	super_class->MarkReferences();
	name->MarkReferences();
	instance_fields->MarkReferences();
	instance_invokables->MarkReferences();
}

int VMClass::numberOfSuperInstanceFields()
{
	if (this->has_super_class()) return this->super_class->get_number_of_instance_fields();
	return 0;
}