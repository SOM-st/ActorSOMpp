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

    VMClass* CompileClass(const StdString& path, const StdString& file,
                                  VMClass* system_class);
    VMClass* CompileClassString(const StdString& stream, VMClass* system_class);
private:
    void showCompilationError(const StdString& filename, const char* message);
    VMClass* compile(VMClass* system_class);
    Parser* parser;
};

#endif
