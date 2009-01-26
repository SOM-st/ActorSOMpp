#pragma once


#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_
#include "VMSymbol.h"
#include "../misc/defs.h"

class Symboltable {
public:
    pVMSymbol lookup(const char* restrict);
    pVMSymbol lookup(const StdString& restrict);
    void      insert(pVMSymbol);

    Symboltable();
    ~Symboltable();
private:
    map<StdString, pVMSymbol> symtab;
};

#endif
