#pragma once

#ifndef SOURCECODECOMPILER_H_
#define SOURCECODECOMPILER_H_
#include "../misc/defs.h"
class VMClass;
class Parser;

class SourcecodeCompiler
{
public:
    SourcecodeCompiler();
    ~SourcecodeCompiler();

    VMClass* CompileClass(const pString& path, const pString& file,
                                  VMClass* system_class);
    VMClass* CompileClassString(const pString& stream, VMClass* system_class);
private:
    void showCompilationError(const pString& filename, const char* message);
    VMClass* compile(VMClass* system_class);
    Parser* parser;
};

#endif
