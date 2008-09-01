#pragma once

#ifndef SIGNATURE_H_
#define SIGNATURE_H_

class VMSymbol;

class Signature
{
public:
    static int GetNumberOfArguments(VMSymbol* sig);
    static bool IsBinary(VMSymbol* sig);

};

#endif
