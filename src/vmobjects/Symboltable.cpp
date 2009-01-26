#include "Symboltable.h"


pVMSymbol Symboltable::lookup(const char* restrict) {
    return symtab[StdString(restrict)];
}


pVMSymbol Symboltable::lookup(const StdString& restrict) {
    return symtab[restrict];
}


void      Symboltable::insert(pVMSymbol sym) {
    symtab[sym->GetStdString()] = sym;
}


Symboltable::Symboltable() {
}


Symboltable::~Symboltable() {
}
