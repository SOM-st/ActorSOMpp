#pragma once


#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_
#include "VMSymbol.h"
#include "../misc/defs.h"

class Symboltable
{
public:
    VMSymbol* lookup(const char* restrict);
    VMSymbol* lookup(const pString& restrict);
    void      insert(VMSymbol*);

    Symboltable();
    ~Symboltable();
private:
    map<std::string, VMSymbol*> symtab;
};

#endif