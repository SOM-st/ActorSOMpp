#pragma once

#include "../misc/defs.h"

class class_generation_context
{
	pVMSymbol name;
    pVMSymbol super_name;
    bool      class_side;
    list<pString>     instance_fields;
    list<pString>     instance_methods;
    list<pString>     class_fields;
    list<pString>     class_methods;
public:
	class_generation_context();
	~class_generation_context();

	void add_instance_field(pString);
	void add_class_field(pString);
	void set_name(pVMSymbol n) { name = n; }
	void set_super_name(pVMSymbol sn) { super_name = sn; }
	void set_class_side(bool cs) { class_side = cs; }
	pVMSymbol get_name(void);
	pVMSymbol get_super_name(void);
	bool get_class_side(void);


};