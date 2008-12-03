#include "ClassGenerationContext.h"
#include "../vmobjects/VMSymbol.h"
#include "../vmobjects/VMObject.h"
#include "../vmobjects/VMClass.h"

ClassGenerationContext::ClassGenerationContext() {
	name = NULL;
	super_name = NULL;
    class_side = false;
	instance_fields.Clear();
    instance_methods.Clear();
	class_fields.Clear();
    class_methods.Clear();
}


ClassGenerationContext::~ClassGenerationContext() {
}


void ClassGenerationContext::AddClassField(VMObject* field) {
	this->class_fields.Add(field);
}


void ClassGenerationContext::AddInstanceField(VMObject* field) {
	this->instance_fields.Add(field);
}


bool ClassGenerationContext::FindField(const StdString& field) {

	pList fields = IsClassSide() ?
        class_fields :
        instance_fields;
    return fields.IndexOf( (VMObject*)_UNIVERSE->SymbolFor(field)) != -1;

}



void ClassGenerationContext::AddInstanceMethod(VMObject* method) {
	this->instance_methods.Add(method);
}



void ClassGenerationContext::AddClassMethod(VMObject* method) {
	this->class_methods.Add(method);
}


VMClass* ClassGenerationContext::Assemble() {
    // build class class name
    StdString ccname = string(name->GetStdString()) + " class";
    
    // Load the super class
    VMClass* super_class = _UNIVERSE->LoadClass(super_name);
    
    // Allocate the class of the resulting class
    VMClass* result_class = _UNIVERSE->NewClass(Globals::MetaClassClass());

    // Initialize the class of the resulting class
    result_class->SetInstanceFields(_UNIVERSE->NewArrayList(class_fields));
    result_class->SetInstanceInvokables(_UNIVERSE->NewArrayList(class_methods));
    result_class->SetName(_UNIVERSE->SymbolFor(ccname));

    VMClass* super_mclass = super_class->GetClass();
    result_class->SetSuperClass(super_mclass);
    
    // Allocate the resulting class
    VMClass* result = _UNIVERSE->NewClass(result_class);
    
    // Initialize the resulting class
    result->SetInstanceFields(_UNIVERSE->NewArrayList(instance_fields));
    result->SetInstanceInvokables(_UNIVERSE->NewArrayList(instance_methods));
    result->SetName(this->name);
    result->SetSuperClass(super_class);
    
    return result;
}



void ClassGenerationContext::AssembleSystemClass( VMClass* system_class ) {
    system_class->SetInstanceInvokables(_UNIVERSE->NewArrayList
                                                        (instance_methods));
    system_class->SetInstanceFields(_UNIVERSE->NewArrayList(instance_fields));
    // class-bound == class-instance-bound 
    VMClass* super_mclass = system_class->GetClass();
    super_mclass->SetInstanceInvokables(_UNIVERSE->NewArrayList(class_methods));
    super_mclass->SetInstanceFields(_UNIVERSE->NewArrayList(class_fields));
}


