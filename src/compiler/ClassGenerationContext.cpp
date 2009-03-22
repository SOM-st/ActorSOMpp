#include "ClassGenerationContext.h"

#include "../vmobjects/VMSymbol.h"
#include "../vmobjects/VMObject.h"
#include "../vmobjects/VMClass.h"

ClassGenerationContext::ClassGenerationContext() :
    instanceFields(), instanceMethods(), classFields(), classMethods(){
	name = NULL;
	superName = NULL;
    classSide = false;
}


ClassGenerationContext::~ClassGenerationContext() {
}


void ClassGenerationContext::AddClassField(pVMObject field) {
	this->classFields.Add(field);
}


void ClassGenerationContext::AddInstanceField(pVMObject field) {
	this->instanceFields.Add(field);
}


bool ClassGenerationContext::FindField(const StdString& field) {

	ExtendedList<pVMObject> fields = IsClassSide() ?
        classFields :
        instanceFields;
    return fields.IndexOf( (pVMObject)_UNIVERSE->SymbolFor(field)) != -1;

}



void ClassGenerationContext::AddInstanceMethod(pVMObject method) {
	this->instanceMethods.Add(method);
}



void ClassGenerationContext::AddClassMethod(pVMObject method) {
	this->classMethods.Add(method);
}


pVMClass ClassGenerationContext::Assemble() {
    // build class class name
    StdString ccname = string(name->GetStdString()) + " class";
    
    // Load the super class
    pVMClass superClass = _UNIVERSE->LoadClass(superName);
    
    // Allocate the class of the resulting class
    pVMClass resultClass = _UNIVERSE->NewClass(metaClassClass);

    // Initialize the class of the resulting class
    resultClass->SetInstanceFields(_UNIVERSE->NewArrayList(classFields));
    resultClass->SetInstanceInvokables(_UNIVERSE->NewArrayList(classMethods));
    resultClass->SetName(_UNIVERSE->SymbolFor(ccname));

    pVMClass superMClass = superClass->GetClass();
    resultClass->SetSuperClass(superMClass);
    
    // Allocate the resulting class
    pVMClass result = _UNIVERSE->NewClass(resultClass);
    
    // Initialize the resulting class
    result->SetInstanceFields(_UNIVERSE->NewArrayList(instanceFields));
    result->SetInstanceInvokables(_UNIVERSE->NewArrayList(instanceMethods));
    result->SetName(this->name);
    result->SetSuperClass(superClass);
    
    return result;
}



void ClassGenerationContext::AssembleSystemClass( pVMClass systemClass ) {
    systemClass->SetInstanceInvokables(_UNIVERSE->NewArrayList
                                                        (instanceMethods));
    systemClass->SetInstanceFields(_UNIVERSE->NewArrayList(instanceFields));
    // class-bound == class-instance-bound 
    pVMClass superMClass = systemClass->GetClass();
    superMClass->SetInstanceInvokables(_UNIVERSE->NewArrayList(classMethods));
    superMClass->SetInstanceFields(_UNIVERSE->NewArrayList(classFields));
}


