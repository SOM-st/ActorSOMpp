#pragma once

#ifndef SYMBOLCOMPARE_H_
#define SYMBOLCOMPARE_H_
class VMString;
//#include "../vmobjects/VMSymbol.h"
struct SymbolCompare{
bool operator()(  VMString* s1,  VMString* s2 ) const; 
};
#endif
