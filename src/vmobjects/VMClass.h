#pragma once

#ifndef VMCLASS_H_
#define VMCLASS_H_
#include <vector>
#include "VMObject.h"
#include "../misc/defs.h"
//#include "../primitives/Core.h"

#if defined(__GNUC__)
#else   //Visual Studio
    #   include <windows.h> 
#endif

class VMSymbol;
class VMArray;
class VMPrimitive;
class ClassGenerationContext;
//class ifstream;

class VMClass : public VMObject{
public:
	VMClass();
    VMClass(int number_of_fields);
	//virtual ~VMClass() {}

	virtual VMClass*  GetSuperClass(); 
    virtual void      SetSuperClass(VMClass*); 
    virtual bool      HasSuperClass(); 
    virtual VMSymbol* GetName(); 
    virtual void      SetName(VMSymbol*); 
    virtual VMArray*  GetInstanceFields(); 
    virtual void      SetInstanceFields(VMArray*); 
    virtual VMArray*  GetInstanceInvokables(); 
    virtual void      SetInstanceInvokables(VMArray*); 
    virtual int       GetNumberOfInstanceInvokables(); 
    virtual VMObject* GetInstanceInvokable(int); 
    virtual void      SetInstanceInvokable(int, VMObject*); 
    virtual VMObject* LookupInvokable(VMSymbol*); 
    virtual int       LookupFieldIndex(VMSymbol*); 
    virtual bool      AddInstanceInvokable(VMObject*); 
    virtual void      AddInstancePrimitive(VMPrimitive*); 
    virtual VMSymbol* GetInstanceFieldName(int); 
    virtual int       GetNumberOfInstanceFields(); 
    virtual bool      HasPrimitives(); 
    virtual void      LoadPrimitives(const vector<pString>&,int);

	//virtual void MarkReferences();
	

private:
	
    
    pString genLoadstring(const pString& cp, 
                       const pString& cname
                       );

    pString genCoreLoadstring(const pString& cp);

    //void* loadLib(const pString& path);
#if defined(__GNUC__)
    void* loadLib(const pString& path);
    bool isResponsible(void* handle, const pString& cl);
    static void set_primitives(VMClass* cl, void* handle, const pString& cname,
                    const char* format
                    );
#else
    HMODULE loadLib(const pString& path);
    bool isResponsible(HMODULE handle, const pString& cl);
    static void set_primitives(VMClass* cl, HMODULE handle, const pString& cname,
                    const char* format
                    );
#endif

    
    
    int numberOfSuperInstanceFields();

	VMClass*  super_class; 
    VMSymbol* name; 
    VMArray*  instance_fields; 
    VMArray*  instance_invokables;
};

#endif
