#pragma once

#ifndef UNIVERSE_H_
#define UNIVERSE_H_

//#define __DEBUG
#include <map>

#include "../misc/defs.h"
#include "../misc/ExtendedList.h"

#include "../interpreter/Interpreter.h"
#include "../memory/Heap.h"
#include "Globals.h"

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

//Convenience macro for Singleton access
#define _UNIVERSE Universe::GetUniverse()
//macro to access the heap
#define _HEAP Heap::GetHeap()



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
    int getClassPathExt(vector<pString>& tokens, const pString& arg);
    
    static Universe *theUniverse;
    
    int setupClassPath(const pString& cp);
    int addClassPath(const pString& cp);
    void printUsageAndExit(char* executable);
	

    void initialize(int, char**);

    //void prepareNilObject();


    
	Heap *heap;
    int heapSize;
	map<pString, VMObject*> globals;
    vector<pString> class_path;
    int cp_count;
    Symboltable* symboltable;
    SourcecodeCompiler* compiler;
    Interpreter* interpreter;
};


#endif
