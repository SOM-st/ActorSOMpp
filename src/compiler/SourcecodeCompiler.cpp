#include "SourcecodeCompiler.h"
#include "../vmobjects/VMClass.h"
#include "../vmobjects/VMSymbol.h"
#include "ClassGenerationContext.h"
#include "Parser.h"
#include <sstream>
#include <fstream>

SourcecodeCompiler::SourcecodeCompiler()
{
    parser = NULL;
}

SourcecodeCompiler::~SourcecodeCompiler()
{
    if (parser != NULL) delete(parser);
}

VMClass* SourcecodeCompiler::compile_class( const pString& path, const pString& file, VMClass* system_class )
{
    VMClass* result = system_class;

    pString fname = path + file_separator + file + ".som";
    std::cout << "compiling " << fname << endl;
    ifstream* fp = new ifstream();
    fp->open(fname.c_str(), std::ios_base::in);
	if (!fp->is_open()) {
		cout << "error opening " << fname <<endl;
		return NULL;
	}

    if (parser != NULL) delete(parser);
    parser = new Parser(*fp);
    result = compile(system_class);

    VMSymbol* cname = result->get_name();
    pString cname_c = cname->GetStdString();
    if (file != cname_c) {
        show_compilation_error(file, "File does not match class name");
        return NULL;
    }
    delete(parser);
    parser = NULL;
    delete(fp);
    return result;
}

VMClass* SourcecodeCompiler::compile_class_string( const pString& stream, VMClass* system_class )
{
    istringstream* ss = new istringstream(stream);
    if (parser != NULL) delete(parser);
    parser = new Parser(*ss);
    
    VMClass* result = compile(system_class);
    delete(parser);
    parser = NULL;
    delete(ss);

    return result;
}

void SourcecodeCompiler::show_compilation_error( const pString& filename, const char* message )
{
    cout << "Error when compiling " << filename << ":" << endl;
    cout << message << endl;
}

VMClass* SourcecodeCompiler::compile( VMClass* system_class )
{
    if (parser == NULL) {
        cout << "Parser not initiated" << endl;
        _UNIVERSE->error_exit("Compiler error");
    }
    class_generation_context* cgc = new class_generation_context();

    VMClass* result = system_class;

    parser->Classdef(cgc);

    if (system_class == NULL) result = cgc->Assemble();
    else cgc->AssembleSystemClass(result);

    delete(cgc);

    std::cout << "Compilation finished" << endl;
    return result;
}

