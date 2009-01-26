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
                                          pVMClass system_class ) {
    pVMClass result = system_class;

    StdString fname = path + file_separator + file + ".som";
#ifdef COMPILER_DEBUG
    std::cout << "compiling " << fname << endl;
#endif
    ifstream* fp = new ifstream();
    fp->open(fname.c_str(), std::ios_base::in);
	if (!fp->is_open()) {
		return NULL;
	}

    if (parser != NULL) delete(parser);
    parser = new Parser(*fp);
    result = compile(system_class);

    pVMSymbol cname = result->GetName();
    StdString cname_c = cname->GetStdString();

    if (file != cname_c) {
        
        ostringstream Str;
        Str << "Filename: " << file << " does not match class name " << cname_c;

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
                                                pVMClass system_class ) {
    istringstream* ss = new istringstream(stream);
    if (parser != NULL) delete(parser);
    parser = new Parser(*ss);
    
    pVMClass result = compile(system_class);
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


pVMClass SourcecodeCompiler::compile( pVMClass system_class ) {
    if (parser == NULL) {
        cout << "Parser not initiated" << endl;
        _UNIVERSE->ErrorExit("Compiler error");
    }
    ClassGenerationContext* cgc = new ClassGenerationContext();

    pVMClass result = system_class;

    parser->Classdef(cgc);

    if (system_class == NULL) result = cgc->Assemble();
    else cgc->AssembleSystemClass(result);

    delete(cgc);

    
    return result;
}

