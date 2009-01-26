#pragma once
#ifndef CLASSGENERATIONCONTEXT_H_
#define CLASSGENERATIONCONTEXT_H_
class VMSymbol;
class VMObject;
class VMClass;
#include "../vmobjects/ObjectFormats.h"
#include "../misc/defs.h"
#include "../misc/ExtendedList.h"

class ClassGenerationContext {
	
public:
	ClassGenerationContext();
	~ClassGenerationContext();
    pVMClass Assemble();
    void AssembleSystemClass(pVMClass system_class);

	bool FindField(const StdString&);
	void AddInstanceField(pVMObject);
	void AddClassField(pVMObject);
	void AddInstanceMethod(pVMObject);
	void AddClassMethod(pVMObject);
	void SetName(pVMSymbol n) { name = n; }
	void SetSuperName(pVMSymbol sn) { super_name = sn; }
	void SetClassSide(bool cs) { class_side = cs; }
	pVMSymbol GetName(void) { return name; };
	pVMSymbol GetSuperName(void) { return super_name; };
	bool IsClassSide(void) { return class_side;};

private:
    pVMSymbol name;
    pVMSymbol super_name;
    bool      class_side;
    ExtendedList<pVMObject>     instance_fields;
    ExtendedList<pVMObject>     instance_methods;
    ExtendedList<pVMObject>     class_fields;
    ExtendedList<pVMObject>     class_methods;

};

#endif
