#pragma once

#ifndef UNIVERSE_H_
#define UNIVERSE_H_

//#define __DEBUG
#include <map>
#include <vector>

#include "../misc/defs.h"
#include "../misc/ExtendedList.h"
#include "../vmobjects/ObjectFormats.h"
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
extern short dumpBytecodes;
extern short gcVerbosity;

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
	map<pVMSymbol, pVMObject>  GetGlobals() {return globals;}
	Heap* GetHeap() {return heap;}
    Interpreter* GetInterpreter() {return interpreter;}

    //

    void          Assert(bool) const;

    pVMSymbol     SymbolFor(const StdString&);
    pVMSymbol     SymbolForChars(const char*);

    //VMObject instanciation methods. These should probably be refactored to a new class
    pVMArray      NewArray(int) const;
    pVMArray      NewArrayList(ExtendedList<pVMObject>& list) const;
    pVMArray      NewArrayFromArgv(const vector<StdString>&) const;
    pVMBlock      NewBlock(pVMMethod, pVMFrame, int);
    pVMClass      NewClass(pVMClass) const;
    pVMFrame      NewFrame(pVMFrame, pVMMethod) const;
    pVMMethod     NewMethod(pVMSymbol, size_t, size_t) const;
    pVMObject     NewInstance(pVMClass) const;
    pVMInteger    NewInteger(int32_t) const;
    pVMBigInteger NewBigInteger(int64_t) const;
    pVMDouble     NewDouble(double) const;
    pVMClass      NewMetaclassClass(void) const;
    pVMString     NewString(const StdString&) const;
    pVMSymbol     NewSymbol(const StdString&);
    pVMString     NewString(const char*) const;
    pVMSymbol     NewSymbol(const char*);
    pVMClass      NewSystemClass(void) const;

    void          InitializeSystemClass(pVMClass, pVMClass, const char*);

    pVMObject     GetGlobal(pVMSymbol);
    void          SetGlobal(pVMSymbol name, pVMObject val);
    bool          HasGlobal(pVMSymbol);

    pVMClass      GetBlockClass(void) const;
    pVMClass      GetBlockClassWithArgs(int);

    pVMClass      LoadClass(pVMSymbol);
    void          LoadSystemClass(pVMClass);
    pVMClass      LoadClassBasic(pVMSymbol, pVMClass);
    pVMClass      LoadShellClass(StdString&);
    
    Universe();
	~Universe();
    //
private:
    vector<StdString>  handleArguments(int argc, char** argv) ;
    int getClassPathExt(vector<StdString>& tokens, const StdString& arg) const;
    
    static Universe* theUniverse;
    
    int setupClassPath(const StdString& cp);
    int addClassPath(const StdString& cp);
    void printUsageAndExit(char* executable) const;
	

    void initialize(int, char**);

    //void prepareNilObject();


    
	Heap* heap;
    int heapSize;
	map<pVMSymbol, pVMObject> globals;
    vector<StdString> classPath;
    
    Symboltable* symboltable;
    SourcecodeCompiler* compiler;
    Interpreter* interpreter;
};


#endif
