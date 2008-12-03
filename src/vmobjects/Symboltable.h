#pragma once


#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_
#include "VMSymbol.h"
#include "../misc/defs.h"

class Symboltable {
public:
    VMSymbol* lookup(const char* restrict);
    VMSymbol* lookup(const StdString& restrict);
    void      insert(VMSymbol*);

    Symboltable();
    ~Symboltable();
private:
    map<StdString, VMSymbol*> symtab;
};

#endif
