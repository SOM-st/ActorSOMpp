#pragma once

#ifndef UNIVERSE_H_
#define UNIVERSE_H_

#include <vector>
#include "../misc/defs.h"
#include "../vmobjects/VMObject.h"
#include "../memory/Heap.h"

typedef struct _globals_entry globals_entry;
struct _globals_entry {
    pString name;
    VMObject *value;
} ;
using namespace std;
class Universe
{
public:
	static Universe* GetUniverse();
	static void InitializeUniverse();

	vector<globals_entry>  GetGlobals() {return globals;}
	Heap* GetHeap() {return heap;}
	void RunGC();
	void SetGlobal(pString name, VMObject* val);
private:
	static Universe *theUniverse;

	Universe();
	~Universe();
	Heap *heap;
	vector<globals_entry> globals;
};

#endif