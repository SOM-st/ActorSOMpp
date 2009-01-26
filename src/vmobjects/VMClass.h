#pragma once

#ifndef VMCLASS_H_
#define VMCLASS_H_
#include <vector>
#include "VMObject.h"
#include "../misc/defs.h"
//#include "../primitives/Core.h"

#if defined(_MSC_VER)   //Visual Studio
    #include <windows.h> 
    #include "../primitives/Core.h"
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

	virtual inline pVMClass  GetSuperClass() const; 
    virtual inline void      SetSuperClass(pVMClass); 
    virtual bool      HasSuperClass() const;  
    virtual inline pVMSymbol GetName() const; 
    virtual inline void      SetName(pVMSymbol);  
    virtual inline pVMArray  GetInstanceFields() const; 
    virtual inline void      SetInstanceFields(pVMArray); 
    virtual inline pVMArray  GetInstanceInvokables() const; 
    virtual void      SetInstanceInvokables(pVMArray); 
    virtual int       GetNumberOfInstanceInvokables() const; 
    virtual pVMObject GetInstanceInvokable(int) const; 
    virtual void      SetInstanceInvokable(int, pVMObject); 
    virtual pVMObject LookupInvokable(pVMSymbol) const; 
    virtual int       LookupFieldIndex(pVMSymbol) const; 
    virtual bool      AddInstanceInvokable(pVMObject); 
    virtual void      AddInstancePrimitive(pVMPrimitive); 
    virtual pVMSymbol GetInstanceFieldName(int)const; 
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
    void* loadLib(const StdString& path) const;
    bool isResponsible(void* handle, const StdString& cl) const;
    void set_primitives(void* handle, const StdString& cname);
//#else
//    HMODULE loadLib(const StdString& path) const;
//    bool isResponsible(HMODULE handle, const StdString& cl) const;
//    void set_primitives(HMODULE handle, const StdString& cname);
//#endif

    
    
    int numberOfSuperInstanceFields() const;

	pVMClass  super_class; 
    pVMSymbol name; 
    pVMArray  instance_fields; 
    pVMArray  instance_invokables;
};


pVMClass VMClass::GetSuperClass() const {
	return super_class;
}


void VMClass::SetSuperClass(pVMClass sup) {
	super_class = sup;
}


pVMSymbol VMClass::GetName()  const {
	return name;
}


void VMClass::SetName(pVMSymbol nam) {
	name = nam;
}


pVMArray VMClass::GetInstanceFields() const {
	return instance_fields;
}


void VMClass::SetInstanceFields(pVMArray inst_fields) {
	instance_fields = inst_fields;
}


VMArray  *VMClass::GetInstanceInvokables() const {
	return instance_invokables;
}

#endif
