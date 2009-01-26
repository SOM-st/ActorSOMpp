#pragma once

#ifndef DISASSEMBELR_H_
#define DISASSEMBELR_H_

#include "../vmobjects/VMClass.h"
#include "../vmobjects/VMMethod.h"
#include "../vmobjects/VMFrame.h"

class Disassembler {
public:
    static void Dump(pVMClass cl);
    static void DumpMethod(pVMMethod method, const char* indent);
    static void DumpBytecode(pVMFrame frame, pVMMethod method, int bc_idx);
private:
    static void dispatch(pVMObject o);
};

#endif

