#include "Universe.h"
#include "../vmobjects/VMSymbol.h"

Universe* Universe::theUniverse;

Universe::Universe()
{
	heap = new Heap();
	VMObject *vmo = new (heap) VMObject;
    VMSymbol *sym = new (heap) VMSymbol("nil");
	SetGlobal(sym, vmo);
}

void Universe::RunGC()
{
	heap->GCCollect();
}

void Universe::SetGlobal(VMSymbol* name, VMObject *val)
{
	globals[name] = val;
}

Universe* Universe::GetUniverse()
{
	if (!theUniverse) theUniverse = new Universe();
	return theUniverse;
}

void Universe::InitializeUniverse()
{
	if (!theUniverse) theUniverse = new Universe();
}