#include "Symboltable.h"


VMSymbol* Symboltable::lookup(const char* restrict) {
    return symtab[StdString(restrict)];
}


VMSymbol* Symboltable::lookup(const StdString& restrict) {
    return symtab[restrict];
}


void      Symboltable::insert(VMSymbol* sym) {
    symtab[sym->GetStdString()] = sym;
}


Symboltable::Symboltable() {
}


Symboltable::~Symboltable() {
}
