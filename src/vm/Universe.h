#pragma once

#ifndef UNIVERSE_H_
#define UNIVERSE_H_

//#define __DEBUG
#include <map>
#include <vector>

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

    //static methods
	static Universe* GetUniverse();
    static void Start(int argc, char** argv);
    static void Quit(int);
    static void ErrorExit(const char*);

    //Globals accessor (only for GC, could be considered to either
    //be moved there or declared as a private friend method for the GC
	map<StdString, VMObject*>  GetGlobals() {return globals;}
	Heap* GetHeap() {return heap;}
    Interpreter* GetInterpreter() {return interpreter;}

    //

    void          Assert(bool) const;

    VMSymbol*     SymbolFor(const StdString&);
    VMSymbol*     SymbolForChars(const char*);

    //VMObject instanciation methods. These should probably be refactored to a new class
    VMArray*      NewArray(int) const;
    VMArray*      NewArrayList(ExtendedList<VMObject*>& list) const;
    VMArray*      NewArrayFromArgv(const vector<StdString>&) const;
    VMBlock*      NewBlock(VMMethod*, VMFrame*, int);
    VMClass*      NewClass(VMClass*) const;
    VMFrame*      NewFrame(VMFrame*, VMMethod*) const;
    VMMethod*     NewMethod(VMSymbol*, size_t, size_t) const;
    VMObject*     NewInstance(VMClass*) const;
    VMInteger*    NewInteger(int32_t) const;
    VMBigInteger* NewBigInteger(int64_t) const;
    VMDouble*     NewDouble(double) const;
    VMClass*      NewMetaclassClass(void) const;
    VMString*     NewString(const StdString&) const;
    VMSymbol*     NewSymbol(const StdString&);
    VMString*     NewString(const char*) const;
    VMSymbol*     NewSymbol(const char*);
    VMClass*      NewSystemClass(void) const;
    
    VMObject*     NewTaggedInteger(int32_t);

    void          InitializeSystemClass(VMClass*, VMClass*, const char*);

    VMObject*     GetGlobal(VMSymbol*);
    void          SetGlobal(VMSymbol* name, VMObject* val);
    bool          HasGlobal(VMSymbol*);

    VMClass*      GetBlockClass(void) const;
    VMClass*      GetBlockClassWithArgs(int);

    VMClass*      LoadClass(VMSymbol*);
    void          LoadSystemClass(VMClass*);
    VMClass*      LoadClassBasic(VMSymbol*, VMClass*);
    VMClass*      LoadShellClass(StdString&);
    
    Universe();
	~Universe();
    //
private:
    vector<StdString>  handleArguments(int argc, char** argv) ;
    int getClassPathExt(vector<StdString>& tokens, const StdString& arg) const;
    
    static Universe *theUniverse;
    
    int setupClassPath(const StdString& cp);
    int addClassPath(const StdString& cp);
    void printUsageAndExit(char* executable) const;
	

    void initialize(int, char**);

    //void prepareNilObject();


    
	Heap *heap;
    int heapSize;
	map<StdString, VMObject*> globals;
    vector<StdString> class_path;
    int cp_count;
    Symboltable* symboltable;
    SourcecodeCompiler* compiler;
    Interpreter* interpreter;
};


#endif
