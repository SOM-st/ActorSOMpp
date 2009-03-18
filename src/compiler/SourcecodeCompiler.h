#pragma once

#ifndef SOURCECODECOMPILER_H_
#define SOURCECODECOMPILER_H_

#include "../misc/defs.h"
#include "../vmobjects/ObjectFormats.h"

class VMClass;
class Parser;

class SourcecodeCompiler
{
public:
    SourcecodeCompiler();
    ~SourcecodeCompiler();

    pVMClass CompileClass(const StdString& path, const StdString& file,
                                  pVMClass systemClass);
    pVMClass CompileClassString(const StdString& stream, pVMClass systemClass);
private:
    void showCompilationError(const StdString& filename, const char* message);
    pVMClass compile(pVMClass systemClass);
    Parser* parser;
};

#endif
