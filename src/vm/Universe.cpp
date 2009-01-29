#include <sstream> 
#include <string.h>
#include <stdlib.h>

#include "Universe.h"
#include "Shell.h"

#include "../vmobjects/VMSymbol.h"
#include "../vmobjects/VMObject.h"
#include "../vmobjects/VMMethod.h"
#include "../vmobjects/VMClass.h"
#include "../vmobjects/VMFrame.h"
#include "../vmobjects/VMArray.h"
#include "../vmobjects/VMBlock.h"
#include "../vmobjects/VMDouble.h"
#include "../vmobjects/VMInteger.h"
#include "../vmobjects/VMString.h"
#include "../vmobjects/VMBigInteger.h"
#include "../vmobjects/VMEvaluationPrimitive.h"
#include "../vmobjects/Symboltable.h"

#include "../interpreter/bytecodes.h"

#include "../compiler/Disassembler.h"
#include "../compiler/SourcecodeCompiler.h"

// Here we go:
// externally refenced variables:
//pVMObject Globals::NilObject() = NULL;
//pVMObject Globals::TrueObject() = NULL;
//pVMObject Globals::FalseObject() = NULL;
//
//pVMClass Globals::ObjectClass() = NULL;
//pVMClass Globals::ClassClass() = NULL;
//pVMClass Globals::MetaClassClass() = NULL;
//
//pVMClass Globals::NilClass() = NULL;
//pVMClass Globals::IntegerClass() = NULL;
//pVMClass Globals::BigIntegerClass() = NULL;
//pVMClass Globals::ArrayClass() = NULL;
//pVMClass Globals::MethodClass() = NULL;
//pVMClass Globals::SymbolClass() = NULL;
//pVMClass Globals::FrameClass() = NULL;
//pVMClass Globals::PrimitiveClass()() = NULL;
//pVMClass Globals::StringClass() = NULL;
//pVMClass Globals::SystemClass() = NULL;
//pVMClass Globals::BlockClass() = NULL;
//pVMClass Globals::DoubleClass() = NULL;

short dump_bytecodes;
short gc_verbosity;


Universe* Universe::theUniverse = NULL;


//Singleton accessor
Universe* Universe::GetUniverse() {
    if (!theUniverse) {
        ErrorExit("Trying to access uninitialized Universe, exiting.");
    }
	return theUniverse;
}


Universe* Universe::operator->() {
    if (!theUniverse) {
        ErrorExit("Trying to access uninitialized Universe, exiting.");
    }
	return theUniverse;
}


void Universe::Start(int argc, char** argv) {
    /*int vm_argc = 0;
    
    vector<StdString> vm_argv = handleArguments(&vm_argc, argc, argv);*/
   // Core::setup();
	theUniverse = new Universe();
    theUniverse->initialize(argc, argv);
}


void Universe::Quit(int err) {
    if (theUniverse) delete(theUniverse);
    
    //Core::tearDown();
    exit(err);
}


void Universe::ErrorExit( const char* err) {
    cout << "Runtime error: " << err << endl;
    Quit(ERR_FAIL);
}

vector<StdString> Universe::handleArguments( int argc, char** argv ) {

    vector<StdString> vm_args = vector<StdString>();
    dump_bytecodes = 0;
    gc_verbosity = 0;
    for (int i = 1; i < argc ; ++i) {
        //cout << argv[i] << endl;
        if (strncmp(argv[i], "-cp", 3) == 0) {
            if ((argc == i + 1) || class_path.size() > 0)
                printUsageAndExit(argv[0]);
            setupClassPath(StdString(argv[++i]));
        } else if (strncmp(argv[i], "-d", 2) == 0) {
            ++dump_bytecodes;
        } else if (strncmp(argv[i], "-g", 2) == 0) {
            ++gc_verbosity;
        } else if (argv[i][0] == '-' && argv[i][1] == 'H') {
            int heap_size = atoi(argv[i] + 2);
            heapSize = heap_size;
        } else if ((strncmp(argv[i], "-h", 2) == 0) ||
            (strncmp(argv[i], "--help", 6) == 0)) {
                printUsageAndExit(argv[0]);
        } else {
            vector<StdString> ext_path_tokens = vector<StdString>(2);
            StdString tmp_string = StdString(argv[i]);
            if (this->getClassPathExt(ext_path_tokens, tmp_string) ==
                                        ERR_SUCCESS) {
                this->addClassPath(ext_path_tokens[0]);
                
                //delete(&ext_path_tokens);
            }
            //Different from CSOM!!!:
            //In CSOM there is an else, where the original filename is pushed into the vm_args.
            //But unlike the class name in ext_path_tokens (ext_path_tokens[1]) that could
            //still have the .som suffix though.
            //So in CPPSOM getClassPathExt will strip the suffix and add it to ext_path_tokens
            //even if there is no new class path present. So we can in any case do the following:
            vm_args.push_back(ext_path_tokens[1]);
        }
    }
    addClassPath(StdString("."));

    return vm_args;
}

int Universe::getClassPathExt(vector<StdString>& tokens,const StdString& arg ) const {
#define EXT_TOKENS 2
    int result = ERR_SUCCESS;
    int fp_index = arg.find_last_of(file_separator);
    int ssep_index = arg.find(".som");
    //cout << "fp: " << fp_index << " ssep: " << ssep_index << endl;
    if (fp_index == StdString::npos) { //no new path
        //different from CSOM (see also HandleArguments):
        //we still want to strip the suffix from the filename, so
        //we set the start to -1, in order to start the substring
        //from character 0. npos is -1 too, but this is to make sure
        fp_index = -1;
        //instead of returning here directly, we have to remember that
        //there is no new class path and return it later
        result = ERR_FAIL;
    } else tokens[0] = arg.substr(0, fp_index);
    
    //adding filename (minus ".som" if present) to second slot
    ssep_index = ( (ssep_index != StdString::npos) && (ssep_index > fp_index)) ?
                 (ssep_index - 1) :
                 arg.length();
    tokens[1] = arg.substr(fp_index + 1, ssep_index - (fp_index));
    return result;
}


int Universe::setupClassPath( const StdString& cp ) {
    try {
        std::stringstream ss ( cp );
        StdString token;

        int i = 0;
        while( ss >> token) {
            class_path.push_back(token);
            ++i;
        }
        this->cp_count = i;

        return ERR_SUCCESS;
    } catch(std::exception e){ 
        return ERR_FAIL;
    }
}


int Universe::addClassPath( const StdString& cp ) {
    class_path.push_back(cp);
    ++this->cp_count;
    return ERR_SUCCESS;
}


void Universe::printUsageAndExit( char* executable ) const {
    cout << "Usage: " << executable << " [-options] [args...]" << endl << endl;
    cout << "where options include:" << endl;
    cout << "    -cp <directories separated by " << path_separator 
         << ">" << endl;
    cout << "        set search path for application classes" << endl;
    //cout << "    -d  enable disassembling (twice for tracing)" << endl;
    /*cout << "    -g  enable garbage collection details:" << endl <<
                    "        1x - print statistics when VM shuts down" << endl <<
                    "        2x - print statistics upon each collection" << endl <<
                    "        3x - print statistics and dump _HEAP upon each "  << endl <<
                    "collection" << endl;*/
    cout << "    -Hx set the _HEAP size to x MB (default: 1 MB)" << endl;
    cout << "    -h  show this help" << endl;

    Quit(ERR_SUCCESS);
}


Universe::Universe(){};


//void Universe::prepareNilObject()
//{
//    Globals::NilObject().SetField(0, Globals::NilObject());
//}


void Universe::initialize(int _argc, char** _argv) {
    heapSize = 1048576;

    vector<StdString> argv = this->handleArguments(_argc, _argv);

    cout << "Setting _HEAP Size to: " << heapSize << endl;
    Heap::InitializeHeap(heapSize);
    heap = _HEAP;
	//_HEAP = new Heap(heapSize);
    symboltable = new Symboltable();
    compiler = new SourcecodeCompiler();
    interpreter = new Interpreter();
    
    //Allocate and initialize the global objects
    cout << "Initializing the global objects" << endl;
    Globals::InitializeGlobals();
    cout << "Done." << endl;
    pVMObject system_object = NewInstance(Globals::SystemClass());

    cout << "Set globals" << endl;
    this->SetGlobal(SymbolForChars("nil"), Globals::NilObject());
    this->SetGlobal(SymbolForChars("true"), Globals::TrueObject());
    this->SetGlobal(SymbolForChars("false"), Globals::FalseObject());
    this->SetGlobal(SymbolForChars("system"), system_object);
    this->SetGlobal(SymbolForChars("System"), Globals::SystemClass());
    this->SetGlobal(SymbolForChars("Block"), Globals::BlockClass());

    cout << "Creating fake bootstrap method" << endl;
    
    pVMMethod bootstrap_method = NewMethod(SymbolForChars("bootstrap"), 1, 0);
    bootstrap_method->SetBytecode(0, BC_HALT);
    bootstrap_method->SetNumberOfLocals(0);
    //bootstrap_method->SetNumberOfArguments(0);
    bootstrap_method->SetMaximumNumberOfStackElements(2);
    bootstrap_method->SetHolder(Globals::SystemClass());
    cout << "Cheer!!! We can start the Interpreter now!" << endl;

    if (argv.size() == 0) {
        Shell* shell = new Shell(bootstrap_method);
        shell->Start();
        return;
    }

    /* only trace bootstrap if the number of cmd-line "-d"s is > 2 */
    short trace = 2 - dump_bytecodes;
    if(!(trace > 0)) dump_bytecodes = 1;

    pVMArray arguments_array = _UNIVERSE->NewArrayFromArgv(argv);
    
    pVMFrame bootstrap_frame = interpreter->PushNewFrame(bootstrap_method);
    bootstrap_frame->Push(system_object);
    bootstrap_frame->Push((pVMObject)arguments_array);

    pVMInvokable initialize = 
        dynamic_cast<pVMInvokable>(Globals::SystemClass()->LookupInvokable(this->SymbolForChars("initialize:")));
    (*initialize)(bootstrap_frame);
    
    // reset "-d" indicator
    if(!(trace>0)) dump_bytecodes = 2 - trace;

    interpreter->Start();
}


Universe::~Universe() {
    if (interpreter) 
        delete(interpreter);
    if (compiler) 
        delete(compiler);
    if (_HEAP) 
        Heap::DestroyHeap();
    if (symboltable) 
        delete(symboltable);
}


pVMObject Universe::NewTaggedInteger(int32_t val) {
    val += 0x80000000;
    return (pVMObject) val;
}


void Universe::Assert( bool value) const {
    if (!value)  {
        cout << "Assertion failed" << endl;
    }

}


pVMClass Universe::GetBlockClass() const {
    return Globals::BlockClass();
}


pVMClass Universe::GetBlockClassWithArgs( int number_of_arguments) {
    this->Assert(number_of_arguments < 10);

    ostringstream Str;
    Str << "Block" << number_of_arguments ;
    StdString block_name(Str.str());
    pVMSymbol name = SymbolFor(block_name);

    if (HasGlobal(name))
        return (pVMClass)GetGlobal(name);

    pVMClass result = LoadClassBasic(name, NULL);

    result->AddInstancePrimitive(new (_HEAP) VMEvaluationPrimitive(number_of_arguments) );

    SetGlobal(name, (pVMObject) result);

    return result;
}



pVMObject Universe::GetGlobal( pVMSymbol name) {
    if (HasGlobal(name))
        return (pVMObject)globals[name->GetStdString()];

    return NULL;
}


bool Universe::HasGlobal( pVMSymbol name) {
    if (globals[name->GetStdString()] != NULL) return true;
    else return false;
}


void Universe::InitializeSystemClass( pVMClass system_class, 
                                     pVMClass super_class, const char* name) {
    StdString s_name(name);

    if (super_class != NULL) {
        system_class->SetSuperClass(super_class);
        pVMClass sys_class_class = system_class->GetClass();
        pVMClass super_class_class = super_class->GetClass();
        sys_class_class->SetSuperClass(super_class_class);
    } else {
        pVMClass sys_class_class = system_class->GetClass();
        sys_class_class->SetSuperClass(Globals::ClassClass());
    }

    pVMClass sys_class_class = system_class->GetClass();

    system_class->SetInstanceFields(NewArray(0));
    sys_class_class->SetInstanceFields(NewArray(0));

    system_class->SetInstanceInvokables(NewArray(0));
    sys_class_class->SetInstanceInvokables(NewArray(0));

    system_class->SetName(SymbolFor(s_name));
    ostringstream Str;
    Str << s_name << " class";
    StdString class_class_name(Str.str());
    sys_class_class->SetName(SymbolFor(class_class_name));

    SetGlobal(system_class->GetName(), (pVMObject)system_class);


}


pVMClass Universe::LoadClass( pVMSymbol name) {
   if (HasGlobal(name))
       return dynamic_cast<pVMClass>(GetGlobal(name));

   pVMClass result = LoadClassBasic(name, NULL);

   if (!result) {
       cout << "can\'t load class " << name->GetStdString() << endl;
       Universe::Quit(ERR_FAIL);
   }

   if (result->HasPrimitives() || result->GetClass()->HasPrimitives())
       result->LoadPrimitives(class_path, cp_count);
    
   return result;
}


pVMClass Universe::LoadClassBasic( pVMSymbol name, pVMClass system_class) {
    StdString s_name = name->GetStdString();
    //cout << s_name.c_str() << endl;
    pVMClass result;

    for (vector<StdString>::iterator i = class_path.begin();
         i != class_path.end(); ++i) {
        result = compiler->CompileClass(*i, name->GetStdString(), system_class);
        if (result) {
            if (dump_bytecodes) {
                Disassembler::Dump(result->GetClass());
                Disassembler::Dump(result);
            }
            return result;
        }

    }
    return NULL;
}


pVMClass Universe::LoadShellClass( StdString& stmt) {
    pVMClass result = compiler->CompileClassString(stmt, NULL);
     if(dump_bytecodes)
         Disassembler::Dump(result);
    return result;
}


void Universe::LoadSystemClass( pVMClass system_class) {
    pVMClass result =
        LoadClassBasic(system_class->GetName(), system_class);
    StdString s = system_class->GetName()->GetStdString();

    if (!result) {
        cout << "Can\'t load system class: " << s;
        Universe::Quit(ERR_FAIL);
    }

    if (result->HasPrimitives() || result->GetClass()->HasPrimitives())
        result->LoadPrimitives(class_path, cp_count);
}


pVMArray Universe::NewArray( int size) const {
    int additionalBytes = size*sizeof(pVMObject);
    pVMArray result = new (_HEAP, additionalBytes) VMArray(size);
    result->SetClass(Globals::ArrayClass());
    return result;
}


pVMArray Universe::NewArrayFromArgv( const vector<StdString>& argv) const {
    pVMArray result = NewArray(argv.size());
    int j = 0;
    for (vector<StdString>::const_iterator i = argv.begin();
         i != argv.end(); ++i) {
        (*result)[j] =  NewString(*i);
        ++j;
    }

    return result;
}


pVMArray Universe::NewArrayList(ExtendedList<pVMObject>& list ) const {
    int size = list.Size();
    pVMArray result = NewArray(size);

    if (result)  {
        for (int i = 0; i < size; ++i) {
            pVMObject elem = list.get(i);
            //result->SetIndexableField(i, elem);
            (*result)[i] = elem;
        }
    }
    return result;
}


pVMBigInteger Universe::NewBigInteger( int64_t value) const {
    pVMBigInteger result = new (_HEAP) VMBigInteger(value);
    result->SetClass(Globals::BigIntegerClass());

    return result;
}


pVMBlock Universe::NewBlock( pVMMethod method, pVMFrame context, int arguments) {
    pVMBlock result = new (_HEAP) VMBlock;
    result->SetClass(this->GetBlockClassWithArgs(arguments));

    result->SetMethod(method);
    result->SetContext(context);

    return result;
}


pVMClass Universe::NewClass( pVMClass class_of_class) const {
    int num_fields = class_of_class->GetNumberOfInstanceFields();
    pVMClass result;
    int additionalBytes = num_fields * sizeof(pVMObject);
    if (num_fields) result = new (_HEAP, additionalBytes) VMClass(num_fields);
    else result = new (_HEAP) VMClass;

    result->SetClass(class_of_class);

    return result;
}


pVMDouble Universe::NewDouble( double value) const {
    pVMDouble result = new (_HEAP) VMDouble(value);
    result->SetClass(Globals::DoubleClass());
    return result;
}


pVMFrame Universe::NewFrame( pVMFrame previous_frame, pVMMethod method) const {
    int length = method->GetNumberOfArguments() +
                 method->GetNumberOfLocals()+
                 method->GetMaximumNumberOfStackElements(); 
   
    int additionalBytes = length * sizeof(pVMObject);
    pVMFrame result = new (_HEAP, additionalBytes) VMFrame(length);
    result->SetClass(Globals::FrameClass());

    result->SetMethod(method);

    if (previous_frame != NULL) 
        result->SetPreviousFrame(previous_frame);

    result->ResetStackPointer();
    result->SetBytecodeIndex(0);

    return result;
}


pVMObject Universe::NewInstance( pVMClass  class_of_instance) const {
    //the number of fields for allocation. We have to calculate the clazz
    //field out of this, because it is already taken care of by VMObject
    int num_of_fields = class_of_instance->GetNumberOfInstanceFields() - 1;
    //the additional space needed is calculated from the number of fields
    int additionalBytes = num_of_fields * sizeof(pVMObject);
    pVMObject result = new (_HEAP, additionalBytes) VMObject(num_of_fields);
    result->SetClass(class_of_instance);
    return result;
}

pVMInteger Universe::NewInteger( int32_t value) const {
    pVMInteger result = new (_HEAP) VMInteger(value);
    result->SetClass(Globals::IntegerClass());
    return result;
}

pVMClass Universe::NewMetaclassClass() const {
    pVMClass result = new (_HEAP) VMClass;
    result->SetClass(new (_HEAP) VMClass);

    pVMClass mclass = result->GetClass();
    mclass->SetClass(result);

    return result;
}


pVMMethod Universe::NewMethod( pVMSymbol signature, 
                    size_t number_of_bytecodes, size_t number_of_constants) const {
    //Method needs space for the bytecodes and the pointers to the constants
    int additionalBytes = number_of_bytecodes + 
                number_of_constants*sizeof(pVMObject);
    pVMMethod result = new (_HEAP,additionalBytes) 
                VMMethod(number_of_bytecodes, number_of_constants);
    result->SetClass(Globals::MethodClass());

    result->SetSignature(signature);

    return result;
}

pVMString Universe::NewString( const StdString& str) const {
    return NewString(str.c_str());
}

pVMString Universe::NewString( const char* str) const {
    //string needs space for str.length characters plus one byte for '\0'
    int additionalBytes = strlen(str) + 1;// str.length() + 1;
    pVMString result = new (_HEAP, additionalBytes) VMString(str);
    result->SetClass(Globals::StringClass());

    return result;
}

pVMSymbol Universe::NewSymbol( const StdString& str) {
    return NewSymbol(str.c_str());
}

pVMSymbol Universe::NewSymbol( const char* str ) {
    //symbol needs space for str.length characters plus one byte for '\0'
    int additionalBytes = strlen(str) + 1; // str.length() + 1;
    pVMSymbol result = new (_HEAP, additionalBytes) VMSymbol(str);
    result->SetClass(Globals::SymbolClass());

    symboltable->insert(result);

    return result;
}


pVMClass Universe::NewSystemClass() const {
    pVMClass system_class = new (_HEAP) VMClass();

    system_class->SetClass(new (_HEAP) VMClass());
    pVMClass mclass = system_class->GetClass();
    
    mclass->SetClass(Globals::MetaClassClass());

    return system_class;
}


pVMSymbol Universe::SymbolFor( const StdString& str) {
    return SymbolForChars(str.c_str());
    
}


pVMSymbol Universe::SymbolForChars( const char* str) {
    pVMSymbol result = symboltable->lookup(str);
    
    return (result != NULL) ?
           result :
           NewSymbol(str);
}


void Universe::SetGlobal(pVMSymbol name, VMObject *val) {
    StdString str =  name->GetStdString();

    globals[str] = val;
	//globals.insert(pair<StdString, pVMObject>(name->GetStdString(), val));
    pVMObject p = globals[str];
    if (p == NULL) {
        cout << "Global " << str << " just added, but globals[" << str << "] == NULL" << endl;
    }
}
