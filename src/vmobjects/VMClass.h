#pragma once

#ifndef VMCLASS_H_
#define VMCLASS_H_
#include <vector>
#include "VMObject.h"
#include "../misc/defs.h"
//#include "../primitives/Core.h"

#if defined(_MSC_VER)   //Visual Studio
    #   include <windows.h> 
#endif

class VMSymbol;
class VMArray;
class VMPrimitive;
class ClassGenerationContext;
//class ifstream;

class VMClass : public VMObject {
public:
	VMClass();
    VMClass(int number_of_fields);
	//virtual ~VMClass() {}

	virtual inline VMClass*  GetSuperClass() const; 
    virtual inline void      SetSuperClass(VMClass*); 
    virtual bool      HasSuperClass() const;  
    virtual inline VMSymbol* GetName() const; 
    virtual inline void      SetName(VMSymbol*);  
    virtual inline VMArray*  GetInstanceFields() const; 
    virtual inline void      SetInstanceFields(VMArray*); 
    virtual inline VMArray*  GetInstanceInvokables() const; 
    virtual void      SetInstanceInvokables(VMArray*); 
    virtual int       GetNumberOfInstanceInvokables() const; 
    virtual VMObject* GetInstanceInvokable(int) const; 
    virtual void      SetInstanceInvokable(int, VMObject*); 
    virtual VMObject* LookupInvokable(VMSymbol*) const; 
    virtual int       LookupFieldIndex(VMSymbol*) const; 
    virtual bool      AddInstanceInvokable(VMObject*); 
    virtual void      AddInstancePrimitive(VMPrimitive*); 
    virtual VMSymbol* GetInstanceFieldName(int)const; 
    virtual int       GetNumberOfInstanceFields() const; 
    virtual bool      HasPrimitives() const; 
    virtual void      LoadPrimitives(const vector<StdString>&,int);

	//virtual void MarkReferences();
	

private:
	
    
    StdString genLoadstring(const StdString& cp, 
                       const StdString& cname
                       ) const;

    StdString genCoreLoadstring(const StdString& cp) const;

    //void* loadLib(const StdString& path);
#if defined(__GNUC__)
    void* loadLib(const StdString& path) const;
    bool isResponsible(void* handle, const StdString& cl) const;
    void set_primitives(void* handle, const StdString& cname);
#else
    HMODULE loadLib(const StdString& path) const;
    bool isResponsible(HMODULE handle, const StdString& cl) const;
    void set_primitives(HMODULE handle, const StdString& cname);
#endif

    
    
    int numberOfSuperInstanceFields() const;

	VMClass*  super_class; 
    VMSymbol* name; 
    VMArray*  instance_fields; 
    VMArray*  instance_invokables;
};


VMClass* VMClass::GetSuperClass() const {
	return super_class;
}


void VMClass::SetSuperClass(VMClass* sup) {
	super_class = sup;
}


VMSymbol* VMClass::GetName()  const {
	return name;
}


void VMClass::SetName(VMSymbol* nam) {
	name = nam;
}


VMArray* VMClass::GetInstanceFields() const {
	return instance_fields;
}


void VMClass::SetInstanceFields(VMArray* inst_fields) {
	instance_fields = inst_fields;
}


VMArray  *VMClass::GetInstanceInvokables() const {
	return instance_invokables;
}

#endif
