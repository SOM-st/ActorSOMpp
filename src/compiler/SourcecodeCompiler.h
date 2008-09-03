#pragma once

#ifndef SOURCECODECOMPILER_H_
#define SOURCECODECOMPILER_H_
#include "../misc/defs.h"
class VMClass;

static class SourcecodeCompiler {
public:
    static VMClass* compile_class(const pString& path, const pString& file,
                                  VMClass* system_class);
    static VMClass* compile_class_string(const pString& stream, VMClass* system_class);
};

#endif