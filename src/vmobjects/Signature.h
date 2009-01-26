#pragma once

#ifndef SIGNATURE_H_
#define SIGNATURE_H_

class VMSymbol;
#include "../vmobjects/ObjectFormats.h"
class Signature {
public:
    static int GetNumberOfArguments(pVMSymbol sig);
    static bool IsBinary(pVMSymbol sig);

};

#endif
