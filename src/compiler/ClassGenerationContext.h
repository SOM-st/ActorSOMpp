#pragma once
#ifndef CLASSGENERATIONCONTEXT_H_
#define CLASSGENERATIONCONTEXT_H_

#include "../misc/defs.h"
#include "GenerationContextCore.h"
class class_generation_context : public generation_context_core 
{
	pVMSymbol name;
    pVMSymbol super_name;
    bool      class_side;
    pList     instance_fields;
    pList     instance_methods;
    pList     class_fields;
    pList     class_methods;
public:
	class_generation_context();
	~class_generation_context();

	bool find_field(pString);
	void add_instance_field(pString);
	void add_class_field(pString);
	void add_instance_method(pString);
	void add_class_method(pString);
	void set_name(pVMSymbol n) { name = n; }
	void set_super_name(pVMSymbol sn) { super_name = sn; }
	void set_class_side(bool cs) { class_side = cs; }
	pVMSymbol get_name(void) { return name; };
	pVMSymbol get_super_name(void) { return super_name; };
	bool is_class_side(void) { return class_side;};


};

#endif
