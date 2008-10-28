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
//#include "../primitives/Core.h"
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
    Universe* operator->();
	static Universe* GetUniverse();
    static void Start(int argc, char** argv);
    static void Quit(int);
    static void ErrorExit(const char*);

	map<pString, VMObject*>  GetGlobals() {return globals;}
	Heap* GetHeap() {return heap;}
    Interpreter* GetInterpreter() {return interpreter;}

    //

    void          Assert(bool);

    VMSymbol*     SymbolFor(const pString&);
    VMSymbol*     SymbolForChars(const char*);

    VMArray*      NewArray(int);
    VMArray*      NewArrayList(pList& list);
    VMArray*      NewArrayFromArgv(const vector<pString>&);
    VMBlock*      NewBlock(VMMethod*, VMFrame*, int);
    VMClass*      NewClass(VMClass*);
    VMFrame*      NewFrame(VMFrame*, VMMethod*);
    VMMethod*     NewMethod(VMSymbol*, size_t, size_t);
    VMObject*     NewInstance(VMClass*);
    VMInteger*    NewInteger(int32_t);
    VMBigInteger* NewBigInteger(int64_t);
    VMDouble*     NewDouble(double);
    VMClass*      NewMetaclassClass(void);
    VMString*     NewString(const pString&);
    VMSymbol*     NewSymbol(const pString&);
    VMClass*      NewSystemClass(void);
    
    VMObject*     NewTaggedInteger(int32_t);
    void          InitializeSystemClass(VMClass*, VMClass*, const char*);

    VMObject*     GetGlobal(VMSymbol*);
    void          SetGlobal(VMSymbol* name, VMObject* val);
    bool          HasGlobal(VMSymbol*);

    VMClass*      GetBlockClass(void);
    VMClass*      GetBlockClassWithArgs(int);

    VMClass*      LoadClass(VMSymbol*);
    void         LoadSystemClass(VMClass*);
    VMClass*      LoadClassBasic(VMSymbol*, VMClass*);
    VMClass*      LoadShellClass(pString&);
    
        Universe();
	~Universe();
    //
private:
    vector<pString>  handleArguments(int argc, char** argv) ;
    int getPathClassExt(vector<pString>& tokens, const pString& arg);
    
    static Universe *theUniverse;
    
    int setupClassPath(const pString& cp);
    int addClassPath(const pString& cp);
    void printUsageAndExit(char* executable);
	

    void initialize(int, char**);

    void prepareNilObject();


    
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
