#pragma once
#ifndef CLASSGENERATIONCONTEXT_H_
#define CLASSGENERATIONCONTEXT_H_
class VMSymbol;
class VMObject;
class VMClass;

#include "../misc/defs.h"
#include "../misc/ExtendedList.h"

class ClassGenerationContext {
	
public:
	ClassGenerationContext();
	~ClassGenerationContext();
    VMClass* Assemble();
    void AssembleSystemClass(VMClass* system_class);

	bool FindField(const StdString&);
	void AddInstanceField(VMObject*);
	void AddClassField(VMObject*);
	void AddInstanceMethod(VMObject*);
	void AddClassMethod(VMObject*);
	void SetName(VMSymbol* n) { name = n; }
	void SetSuperName(VMSymbol* sn) { super_name = sn; }
	void SetClassSide(bool cs) { class_side = cs; }
	VMSymbol* GetName(void) { return name; };
	VMSymbol* GetSuperName(void) { return super_name; };
	bool IsClassSide(void) { return class_side;};

private:
    VMSymbol* name;
    VMSymbol* super_name;
    bool      class_side;
    ExtendedList<VMObject*>     instance_fields;
    ExtendedList<VMObject*>     instance_methods;
    ExtendedList<VMObject*>     class_fields;
    ExtendedList<VMObject*>     class_methods;

};

#endif
