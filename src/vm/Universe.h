#pragma once

#ifndef UNIVERSE_H_
#define UNIVERSE_H_
//#define __DEBUG
//#include <map>
#include "../misc/defs.h"
#include "../misc/ExtendedList.h"
#include <map>
//#include "../vmobjects/VMObject.h"
//#include "../vmobjects/VMSymbol.h"
class VMObject;
class VMSymbol;
class VMClass;
class VMFrame;
class VMArray;
class VMBlock;
class VMDouble;
class VMInteger;
class VMMethod;
class VMString;
class VMBigInteger;
class Symboltable;
class SourcecodeCompiler;
#include "../interpreter/Interpreter.h"
#include "../memory/Heap.h"
#include "../primitives/Core.h"
#include "../misc/SymbolCompare.h"
#define _HEAP Universe::GetUniverse()->GetHeap()
#define _UNIVERSE Universe::GetUniverse()

//global objects
extern VMObject* nil_object;
extern VMObject* true_object;
extern VMObject* false_object;
  
extern VMClass* object_class;
extern VMClass* class_class;
extern VMClass* metaclass_class;
  
extern VMClass* nil_class;
extern VMClass* integer_class;
extern VMClass* biginteger_class;
extern VMClass* array_class;
extern VMClass* method_class;
extern VMClass* symbol_class;
extern VMClass* frame_class;
extern VMClass* primitive_class;
extern VMClass* string_class;
extern VMClass* system_class;
extern VMClass* block_class;
extern VMClass* double_class;

// for runtime debug
extern short dump_bytecodes;
extern short gc_verbosity;

using namespace std;
class Universe
{
public:
	static Universe* GetUniverse();
    static void start(int argc, char** argv);
    static void quit(int);
    static void error_exit(const char*);

	map<pString, VMObject*>  GetGlobals() {return globals;}
	Heap* GetHeap() {return heap;}
    Interpreter* GetInterpreter() {return interpreter;}
	void RunGC();
	

    //

    void          Assert(bool);

    VMSymbol*     symbol_for(const pString&);
    VMSymbol*     symbol_for_chars(const char*);

    VMArray*      new_array(int);
    VMArray*      new_array_list(pList& list);
    VMArray*      new_array_from_argv(int, const vector<pString>&);
    VMBlock*      new_block(VMMethod*, VMFrame*, int);
    VMClass*      new_class(VMClass*);
    VMFrame*      new_frame(VMFrame*, VMMethod*);
    VMMethod*     new_method(VMSymbol*, size_t, size_t);
    VMObject*     new_instance(VMClass*);
    VMInteger*    new_integer(int32_t);
    VMBigInteger* new_biginteger(int64_t);
    VMDouble*     new_double(double);
    VMClass*      new_metaclass_class(void);
    VMString*     new_string(const pString&);
    VMSymbol*     new_symbol(const pString&);
    VMClass*      new_system_class(void);

    void          initialize_system_class(VMClass*, VMClass*, const char*);

    VMObject*     get_global(VMSymbol*);
    void          set_global(VMSymbol* name, VMObject* val);
    bool          has_global(VMSymbol*);

    VMClass*      get_block_class(void);
    VMClass*      get_block_class_with_args(int);

    VMClass*      load_class(VMSymbol*);
    void         load_system_class(VMClass*);
    VMClass*      load_class_basic(VMSymbol*, VMClass*);
    VMClass*      load_shell_class(pString&);
    
    //
private:
    vector<pString>  handle_arguments(int* vm_argc, int argc, char** argv) ;
    int get_path_class_ext(vector<pString>& tokens, const pString& arg);
    
    static Universe *theUniverse;
    
    int setup_class_path(const pString& cp);
    int add_class_path(const pString& cp);
    void print_usage_and_exit(char* executable);
	

    void initialize(int, char**);

    void prepareNilObject();

    Universe();
	~Universe();
    
	Heap *heap;
    int heapSize;
	map<pString, VMObject*> globals;
    vector<pString> class_path;
    int cp_count;
    Symboltable* symboltable;
    SourcecodeCompiler* compiler;
    Interpreter* interpreter;
};/*
/*  Global objects  
extern pVMObject nil_object;
extern pVMObject true_object;
extern pVMObject false_object;
  
extern pVMClass object_class;
extern pVMClass class_class;
extern pVMClass metaclass_class;
  
extern pVMClass nil_class;
extern pVMClass integer_class;
extern pVMClass biginteger_class;
extern pVMClass array_class;
extern pVMClass method_class;
extern pVMClass symbol_class;
extern pVMClass frame_class;
extern pVMClass primitive_class;
extern pVMClass string_class;
extern pVMClass system_class;
extern pVMClass block_class;
extern pVMClass double_class;

// for runtime debug
extern short dump_bytecodes;

// config
extern int universe_use_threaded_interpretation;
extern int universe_super_inst_config_mask; 
*/

#endif
