#include "Universe.h"
#include "../vmobjects/VMObject.h"

Universe* Universe::theUniverse;

Universe::Universe()
{
	heap = new Heap();
	VMObject *vmo = new (heap) VMObject;
	SetGlobal("nil", vmo);
}

void Universe::RunGC()
{
	heap->GCCollect();
}

void Universe::SetGlobal(pString name, VMObject *val)
{
	globals_entry ge;
	ge.name = name;
	ge.value = val;
	globals.push_back(ge);
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