#include "Symboltable.h"

VMSymbol* Symboltable::lookup(const char* restrict)
{
    return symtab[pString(restrict)];
}

VMSymbol* Symboltable::lookup(const pString& restrict)
{
    return symtab[restrict];
}

void      Symboltable::insert(VMSymbol* sym)
{
    symtab[sym->GetStdString()] = sym;
}

Symboltable::Symboltable()
{
}

Symboltable::~Symboltable()
{
}