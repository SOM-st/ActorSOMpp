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
    VMClass(int numberOfFields);
	//virtual ~VMClass() {}

	virtual inline pVMClass  GetSuperClass() const; 
    virtual inline void      SetSuperClass(pVMClass); 
    virtual bool             HasSuperClass() const;  
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
    virtual void      LoadPrimitives(const vector<StdString>&);

	//virtual void MarkReferences();
	

private:
	
    
    StdString genLoadstring(const StdString& cp, 
                       const StdString& cname
                       ) const;

    StdString genCoreLoadstring(const StdString& cp) const;

    //void* loadLib(const StdString& path);
    void* loadLib(const StdString& path) const;
    bool isResponsible(void* handle, const StdString& cl) const;
    void setPrimitives(void* handle, const StdString& cname);
   
    
    int numberOfSuperInstanceFields() const;

	pVMClass  superClass; 
    pVMSymbol name; 
    pVMArray  instanceFields; 
    pVMArray  instanceInvokables;

    static const int VMClassNumberOfFields;
};


pVMClass VMClass::GetSuperClass() const {
	return superClass;
}


void VMClass::SetSuperClass(pVMClass sup) {
	superClass = sup;
}


pVMSymbol VMClass::GetName()  const {
	return name;
}


void VMClass::SetName(pVMSymbol nam) {
	name = nam;
}


pVMArray VMClass::GetInstanceFields() const {
	return instanceFields;
}


void VMClass::SetInstanceFields(pVMArray instFields) {
	instanceFields = instFields;
}


pVMArray VMClass::GetInstanceInvokables() const {
	return instanceInvokables;
}

#endif
