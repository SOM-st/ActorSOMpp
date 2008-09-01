#pragma once


#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_
#include "VMSymbol.h"


class Symboltable
{
public:
    VMSymbol* lookup(const char* restrict);
    VMSymbol* lookup(std::string restrict);
    void      insert(VMSymbol*);

    Symboltable();
    ~Symboltable();
private:
    map<std::string, VMSymbol*> symtab;
};

#endif