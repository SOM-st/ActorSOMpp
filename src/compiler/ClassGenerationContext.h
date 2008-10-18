#pragma once
#ifndef CLASSGENERATIONCONTEXT_H_
#define CLASSGENERATIONCONTEXT_H_
class VMSymbol;
class VMObject;
class VMClass;

#include "../misc/defs.h"
#include "GenerationContextCore.h"
class ClassGenerationContext : public GenerationContextCore 
{
	VMSymbol* name;
    VMSymbol* super_name;
    bool      class_side;
    pList     instance_fields;
    pList     instance_methods;
    pList     class_fields;
    pList     class_methods;
public:
	ClassGenerationContext();
	~ClassGenerationContext();
    VMClass* Assemble();
    void AssembleSystemClass(VMClass* system_class);

	bool FindField(const pString&);
	void add_instance_field(VMObject*);
	void add_class_field(VMObject*);
	void add_instance_method(VMObject*);
	void add_class_method(VMObject*);
	void SetName(VMSymbol* n) { name = n; }
	void set_super_name(VMSymbol* sn) { super_name = sn; }
	void set_class_side(bool cs) { class_side = cs; }
	VMSymbol* GetName(void) { return name; };
	VMSymbol* get_super_name(void) { return super_name; };
	bool is_class_side(void) { return class_side;};


};

#endif
