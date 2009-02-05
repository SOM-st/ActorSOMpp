#include "ClassGenerationContext.h"
#include "../vmobjects/VMSymbol.h"
#include "../vmobjects/VMObject.h"
#include "../vmobjects/VMClass.h"

ClassGenerationContext::ClassGenerationContext() :
    instance_fields(), instance_methods(), class_fields(), class_methods(){
	name = NULL;
	super_name = NULL;
    class_side = false;
}


ClassGenerationContext::~ClassGenerationContext() {
}


void ClassGenerationContext::AddClassField(pVMObject field) {
	this->class_fields.Add(field);
}


void ClassGenerationContext::AddInstanceField(pVMObject field) {
	this->instance_fields.Add(field);
}


bool ClassGenerationContext::FindField(const StdString& field) {

	ExtendedList<pVMObject> fields = IsClassSide() ?
        class_fields :
        instance_fields;
    return fields.IndexOf( (pVMObject)_UNIVERSE->SymbolFor(field)) != -1;

}



void ClassGenerationContext::AddInstanceMethod(pVMObject method) {
	this->instance_methods.Add(method);
}



void ClassGenerationContext::AddClassMethod(pVMObject method) {
	this->class_methods.Add(method);
}


pVMClass ClassGenerationContext::Assemble() {
    // build class class name
    StdString ccname = string(name->GetStdString()) + " class";
    
    // Load the super class
    pVMClass super_class = _UNIVERSE->LoadClass(super_name);
    
    // Allocate the class of the resulting class
    pVMClass result_class = _UNIVERSE->NewClass(Globals::MetaClassClass());

    // Initialize the class of the resulting class
    result_class->SetInstanceFields(_UNIVERSE->NewArrayList(class_fields));
    result_class->SetInstanceInvokables(_UNIVERSE->NewArrayList(class_methods));
    result_class->SetName(_UNIVERSE->SymbolFor(ccname));

    pVMClass super_mclass = super_class->GetClass();
    result_class->SetSuperClass(super_mclass);
    
    // Allocate the resulting class
    pVMClass result = _UNIVERSE->NewClass(result_class);
    
    // Initialize the resulting class
    result->SetInstanceFields(_UNIVERSE->NewArrayList(instance_fields));
    result->SetInstanceInvokables(_UNIVERSE->NewArrayList(instance_methods));
    result->SetName(this->name);
    result->SetSuperClass(super_class);
    
    return result;
}



void ClassGenerationContext::AssembleSystemClass( pVMClass system_class ) {
    system_class->SetInstanceInvokables(_UNIVERSE->NewArrayList
                                                        (instance_methods));
    system_class->SetInstanceFields(_UNIVERSE->NewArrayList(instance_fields));
    // class-bound == class-instance-bound 
    pVMClass super_mclass = system_class->GetClass();
    super_mclass->SetInstanceInvokables(_UNIVERSE->NewArrayList(class_methods));
    super_mclass->SetInstanceFields(_UNIVERSE->NewArrayList(class_fields));
}


