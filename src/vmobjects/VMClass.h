#pragma once

#ifndef VMCLASS_H_
#define VMCLASS_H_
#include <vector>
#include "VMObject.h"
#include "../misc/defs.h"
class VMSymbol;
class VMArray;
class VMPrimitive;
class class_generation_context;
//class ifstream;

class VMClass : public VMObject{
public:
	VMClass();
    VMClass(int number_of_fields);
	virtual ~VMClass() {}

	virtual VMClass*  get_super_class(); 
    virtual void      set_super_class(VMClass*); 
    virtual bool      has_super_class(); 
    virtual VMSymbol* get_name(); 
    virtual void      set_name(VMSymbol*); 
    virtual VMArray*  get_instance_fields(); 
    virtual void      set_instance_fields(VMArray*); 
    virtual VMArray*  get_instance_invokables(); 
    virtual void      set_instance_invokables(VMArray*); 
    virtual int       get_number_of_instance_invokables(); 
    virtual VMObject* get_instance_invokable(int); 
    virtual void      set_instance_invokable(int, VMObject*); 
    virtual VMObject* lookup_invokable(VMSymbol*); 
    virtual int       lookup_field_index(VMSymbol*); 
    virtual bool      add_instance_invokable(VMObject*); 
    virtual void      add_instance_primitive(VMPrimitive*); 
    virtual VMSymbol* get_instance_field_name(int); 
    virtual int       get_number_of_instance_fields(); 
    virtual bool      has_primitives(); 
    virtual void      load_primitives(const vector<pString>&,int);

	virtual void MarkReferences();
	

private:
	int numberOfSuperInstanceFields();
    
    pString gen_loadstring(const pString& cp, 
                       const pString& cname
                       );

    pString gen_core_loadstring(const pString& cp);

    ifstream* load_lib(const pString& path);

    bool is_responsible(ifstream* handle, const pString& cl);
    void set_primitives(VMClass* cl, ifstream* handle, const pString& cname,
                    const char* format
                    );

	VMClass*  super_class; 
    VMSymbol* name; 
    VMArray*  instance_fields; 
    VMArray*  instance_invokables;
};

#endif
