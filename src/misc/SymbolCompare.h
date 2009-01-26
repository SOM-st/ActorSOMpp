#pragma once

#ifndef SYMBOLCOMPARE_H_
#define SYMBOLCOMPARE_H_
#include "../vmobjects/ObjectFormats.h"
class VMString;
//#include "../vmobjects/VMSymbol.h"
struct SymbolCompare{
bool operator()(  pVMString s1,  pVMString s2 ) const; 
};
#endif
