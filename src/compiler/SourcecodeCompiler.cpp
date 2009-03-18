#include <sstream>
#include <fstream>

#include "SourcecodeCompiler.h"
#include "ClassGenerationContext.h"
#include "Parser.h"

#include "../vmobjects/VMClass.h"
#include "../vmobjects/VMSymbol.h"

//#define COMPILER_DEBUG


SourcecodeCompiler::SourcecodeCompiler() {
    parser = NULL;
}


SourcecodeCompiler::~SourcecodeCompiler() {
    if (parser != NULL) delete(parser);
}


pVMClass SourcecodeCompiler::CompileClass( const StdString& path, 
                                          const StdString& file, 
                                          pVMClass systemClass ) {
    pVMClass result = systemClass;

    StdString fname = path + fileSeparator + file + ".som";

    ifstream* fp = new ifstream();
    fp->open(fname.c_str(), std::ios_base::in);
	if (!fp->is_open()) {
		return NULL;
	}

    if (parser != NULL) delete(parser);
    parser = new Parser(*fp);
    result = compile(systemClass);

    pVMSymbol cname = result->GetName();
    StdString cnameC = cname->GetStdString();

    if (file != cnameC) {
        
        ostringstream Str;
        Str << "Filename: " << file << " does not match class name " << cnameC;

        showCompilationError(file, Str.str().c_str());
        return NULL;
    }
    delete(parser);
    parser = NULL;
    delete(fp);
#ifdef COMPILER_DEBUG
    std::cout << "Compilation finished" << endl;
#endif
    return result;
}


pVMClass SourcecodeCompiler::CompileClassString( const StdString& stream, 
                                                pVMClass systemClass ) {
    istringstream* ss = new istringstream(stream);
    if (parser != NULL) delete(parser);
    parser = new Parser(*ss);
    
    pVMClass result = compile(systemClass);
    delete(parser);
    parser = NULL;
    delete(ss);

    return result;
}


void SourcecodeCompiler::showCompilationError( const StdString& filename, 
                                              const char* message ) {
    cout << "Error when compiling " << filename << ":" << endl;
    cout << message << endl;
}


pVMClass SourcecodeCompiler::compile( pVMClass systemClass ) {
    if (parser == NULL) {
        cout << "Parser not initiated" << endl;
        _UNIVERSE->ErrorExit("Compiler error");
    }
    ClassGenerationContext* cgc = new ClassGenerationContext();

    pVMClass result = systemClass;

    parser->Classdef(cgc);

    if (systemClass == NULL) result = cgc->Assemble();
    else cgc->AssembleSystemClass(result);

    delete(cgc);

    
    return result;
}

