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
    void AssembleSystemClass(pVMClass systemClass);

	bool FindField(const StdString&);
	void AddInstanceField(pVMObject);
	void AddClassField(pVMObject);
	void AddInstanceMethod(pVMObject);
	void AddClassMethod(pVMObject);
	void SetName(pVMSymbol n) { name = n; }
	void SetSuperName(pVMSymbol sn) { superName = sn; }
	void SetClassSide(bool cs) { classSide = cs; }
	pVMSymbol GetName(void) { return name; };
	pVMSymbol GetSuperName(void) { return superName; };
	bool IsClassSide(void) { return classSide;};

private:
    pVMSymbol name;
    pVMSymbol superName;
    bool      classSide;
    ExtendedList<pVMObject>     instanceFields;
    ExtendedList<pVMObject>     instanceMethods;
    ExtendedList<pVMObject>     classFields;
    ExtendedList<pVMObject>     classMethods;

};

#endif
