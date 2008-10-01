#pragma once

#ifndef DISASSEMBELR_H_
#define DISASSEMBELR_H_

#include "../vmobjects/VMClass.h"
#include "../vmobjects/VMMethod.h"
#include "../vmobjects/VMFrame.h"

class Disassembler {
public:
    static void Dump(VMClass* cl);
    static void DumpMethod(VMMethod* method, const char* indent);
    static void DumpBytecode(VMFrame* frame, VMMethod* method, int bc_idx);
private:
    static void dispatch(VMObject* o);
};

#endif

