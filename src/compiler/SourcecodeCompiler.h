#pragma once

#ifndef SOURCECODECOMPILER_H_
#define SOURCECODECOMPILER_H_
#include "../misc/defs.h"
class VMClass;
class Parser;

class SourcecodeCompiler {
public:
    SourcecodeCompiler();
    ~SourcecodeCompiler();

    VMClass* compile_class(const pString& path, const pString& file,
                                  VMClass* system_class);
    VMClass* compile_class_string(const pString& stream, VMClass* system_class);
private:
    void show_compilation_error(const pString& filename, const char* message);
    VMClass* compile(VMClass* system_class);
    Parser* parser;
};

#endif
