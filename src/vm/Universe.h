#pragma once

#ifndef UNIVERSE_H_
#define UNIVERSE_H_

//#include <map>
#include "../misc/defs.h"
#include "../vmobjects/VMObject.h"
//#include "../vmobjects/VMSymbol.h"
class VMSymbol;
#include "../memory/Heap.h"

#define _HEAP Universe::GetUniverse()->GetHeap()

using namespace std;
class Universe
{
public:
	static Universe* GetUniverse();
	static void InitializeUniverse();

	map<VMSymbol*, VMObject*>  GetGlobals() {return globals;}
	Heap* GetHeap() {return heap;}
	void RunGC();
	void SetGlobal(VMSymbol* name, VMObject* val);
private:
	static Universe *theUniverse;

	Universe();
	~Universe();
	Heap *heap;
	map<VMSymbol*, VMObject*> globals;
};

#endif