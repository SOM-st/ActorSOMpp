#pragma once

#ifndef UNIVERSE_H_
#define UNIVERSE_H_

#include "../misc/HashMap.h"
#include "../misc/defs.h"
#include "../vmobjects/OOObject.h"

class Universe
{
public:
	//static Universe* GetUniverse();

	HashMap<pString, OOObject*, OOObject>  GetGlobals();
	void SetGlobal(pString name, OOObject* val);
private:
	//static Universe *theUniverse;

	Universe();
	~Universe();
	
	//HashMap<pString, OOObject*, OOObject> globals;
};

#endif