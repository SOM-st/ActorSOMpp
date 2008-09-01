#include "Symboltable.h"

VMSymbol* Symboltable::lookup(const char* restrict)
{
    return symtab[std::string(restrict)];
}

VMSymbol* Symboltable::lookup(std::string restrict)
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