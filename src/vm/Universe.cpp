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
VMObject* nil_object = NULL;
VMObject* true_object = NULL;
VMObject* false_object = NULL;

VMClass* object_class = NULL;
VMClass* class_class = NULL;
VMClass* metaclass_class = NULL;

VMClass* nil_class = NULL;
VMClass* integer_class = NULL;
VMClass* biginteger_class = NULL;
VMClass* array_class = NULL;
VMClass* method_class = NULL;
VMClass* symbol_class = NULL;
VMClass* frame_class = NULL;
VMClass* primitive_class = NULL;
VMClass* string_class = NULL;
VMClass* system_class = NULL;
VMClass* block_class = NULL;
VMClass* double_class = NULL;

short dump_bytecodes;
short gc_verbosity;


Universe* Universe::theUniverse = NULL;


//Singleton accessor
Universe* Universe::GetUniverse()
{
    if (!theUniverse)
    {
        ErrorExit("Trying to access uninitialized Universe, exiting.");
    }
	return theUniverse;
}


Universe* Universe::operator->()
{
    if (!theUniverse) 
    {
        ErrorExit("Trying to access uninitialized Universe, exiting.");
    }
	return theUniverse;
}


vector<pString> Universe::handleArguments( int argc, char** argv )
{

    vector<pString> vm_args = vector<pString>();
    dump_bytecodes = 0;
    gc_verbosity = 0;
    for (int i = 1; i < argc ; ++i)
    {
        if (strncmp(argv[i], "-cp", 3) == 0) 
        {
            if ((argc == i + 1) || class_path.size() > 0)
                printUsageAndExit(argv[0]);
            setupClassPath(pString(argv[++i]));
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
            vector<pString> ext_path_tokens = vector<pString>(2);
            pString tmp_string = pString(argv[i]);
            if (this->getPathClassExt(ext_path_tokens, tmp_string) == ERR_SUCCESS)
            {
                this->addClassPath(ext_path_tokens[0]);
                vm_args.push_back(ext_path_tokens[1]);
                //delete(&ext_path_tokens);
            } else {
                vm_args.push_back(pString(tmp_string));
            }
            //delete(&tmp_string);
            //delete(&ext_path_tokens);
        }
    }
    addClassPath(pString("."));

    return vm_args;
}


void Universe::Start(int argc, char** argv)
{
    /*int vm_argc = 0;
    
    vector<pString> vm_argv = handleArguments(&vm_argc, argc, argv);*/
   // Core::setup();
	theUniverse = new Universe();
    theUniverse->initialize(argc, argv);
}


void Universe::Quit(int err)
{
    if (theUniverse) delete(theUniverse);
    
    //Core::tearDown();
    exit(err);
}


void Universe::ErrorExit( const char* err)
{
    cout << "Runtime error: " << err << endl;
    Quit(ERR_FAIL);
}


int Universe::getPathClassExt(vector<pString>& tokens,const pString& arg )
{
#define EXT_TOKENS 2
    
    int fp_index = arg.find_last_of(file_separator);
    int ssep_index = arg.find(".som");

    if (fp_index == pString::npos) 
    { //no new path
        return ERR_FAIL;
    } else tokens[0] = arg.substr(0, fp_index);
    
    //adding filename (minus ".som" if present) to second slot
    ssep_index = ( (ssep_index != pString::npos) && (ssep_index > fp_index)) ?
                 (ssep_index - 1) :
                 arg.length();
    tokens[1] = arg.substr(fp_index + 1, ssep_index - (fp_index));
    return ERR_SUCCESS;
}


int Universe::setupClassPath( const pString& cp )
{
    try{
        std::stringstream ss ( cp );
        std::string token;

        int i = 0;
        while( ss >> token)
        {
            class_path.push_back(token);
            ++i;
        }
        cp_count = i;

        return ERR_SUCCESS;
    } catch(std::exception e){ 
        return ERR_FAIL;
    }
}


int Universe::addClassPath( const pString& cp )
{
    class_path.push_back(cp);
    ++cp_count;
    return ERR_SUCCESS;
}


void Universe::printUsageAndExit( char* executable )
{
    cout << "Usage: " << executable << " [-options] [args...]" << endl << endl;
    cout << "where options include:" << endl;
    cout << "    -cp <directories separated by " << path_separator << ">" << endl;
    cout << "        set search path for application classes" << endl;
    //cout << "    -d  enable disassembling (twice for tracing)" << endl;
    /*cout << "    -g  enable garbage collection details:" << endl <<
                    "        1x - print statistics when VM shuts down" << endl <<
                    "        2x - print statistics upon each collection" << endl <<
                    "        3x - print statistics and dump heap upon each "  << endl <<
                    "collection" << endl;*/
    cout << "    -Hx set the heap size to x MB (default: 1 MB)" << endl;
    cout << "    -h  show this help" << endl;

    Quit(ERR_SUCCESS);
}


Universe::Universe(){};


void Universe::prepareNilObject()
{
    nil_object->SetField(0, nil_object);
}


void Universe::initialize(int _argc, char** _argv)
{
    heapSize = 1048576;

    vector<pString> argv = this->handleArguments(_argc, _argv);

    cout << "Setting heap Size to: " << heapSize << endl;
	heap = new Heap(heapSize);
    symboltable = new Symboltable();
    compiler = new SourcecodeCompiler();
    interpreter = new Interpreter();
	nil_object = new (heap) VMObject;
    prepareNilObject();
    cout << "We have a nil_object" << endl;
    /*VMArray* vmo = new (heap, 4*sizeof(VMObject*)) VMArray(4);
    VMMethod*/
    metaclass_class = this->NewMetaclassClass();
    cout << "Metaclass Class created, creating System classes" << endl;
    object_class    = this->NewSystemClass();
    nil_class       = this->NewSystemClass();
    class_class     = this->NewSystemClass();
    array_class     = this->NewSystemClass();
    symbol_class    = this->NewSystemClass();
    method_class    = this->NewSystemClass();
    integer_class   = this->NewSystemClass();
    biginteger_class= this->NewSystemClass();
    frame_class     = this->NewSystemClass();
    primitive_class = this->NewSystemClass();
    string_class    = this->NewSystemClass();
    double_class    = this->NewSystemClass();
    cout << "System classes created" << endl;
    
    nil_object->SetClass(nil_class);

    cout << "Initialize System Classes" << endl;
    this->InitializeSystemClass(object_class, NULL, "Object");
    this->InitializeSystemClass(class_class, object_class, "Class");
    this->InitializeSystemClass(metaclass_class, class_class, "Metaclass");
    this->InitializeSystemClass(nil_class, object_class, "Nil");
    this->InitializeSystemClass(array_class, object_class, "Array");
    this->InitializeSystemClass(method_class, array_class, "Method");
    this->InitializeSystemClass(symbol_class, object_class, "Symbol");
    this->InitializeSystemClass(integer_class, object_class, "Integer");
    this->InitializeSystemClass(biginteger_class, object_class,
                                     "BigInteger");
    this->InitializeSystemClass(frame_class, array_class, "Frame");
    this->InitializeSystemClass(primitive_class, object_class,
                                     "Primitive");
    this->InitializeSystemClass(string_class, object_class, "String");
    this->InitializeSystemClass(double_class, object_class, "Double");

    cout << "System classes initialized, now let's load them!"
            << endl;

    this->LoadSystemClass(object_class);
    this->LoadSystemClass(class_class);
    this->LoadSystemClass(metaclass_class);
    this->LoadSystemClass(nil_class);
    this->LoadSystemClass(array_class);
    this->LoadSystemClass(method_class);
    this->LoadSystemClass(symbol_class);
    this->LoadSystemClass(integer_class);
    this->LoadSystemClass(biginteger_class);
    this->LoadSystemClass(frame_class);
    this->LoadSystemClass(primitive_class);
    this->LoadSystemClass(string_class);
    this->LoadSystemClass(double_class);

    cout << "YAY, the system classes are loaded" << endl;
    
    cout << "loading block class" << endl;
    block_class = LoadClass(SymbolForChars("Block"));
    
    cout << "setting up true and false" << endl;
    true_object = NewInstance(LoadClass(SymbolForChars("True")));
    false_object = NewInstance(LoadClass(SymbolForChars("False")));

    cout << "load System" << endl;
    system_class = LoadClass(SymbolForChars("System"));
    VMObject* system_object = NewInstance(system_class);

    cout << "Set globals" << endl;
    this->SetGlobal(SymbolForChars("nil"), nil_object);
    this->SetGlobal(SymbolForChars("true"), true_object);
    this->SetGlobal(SymbolForChars("false"), false_object);
    this->SetGlobal(SymbolForChars("system"), system_object);
    this->SetGlobal(SymbolForChars("System"), system_class);
    this->SetGlobal(SymbolForChars("Block"), block_class);

    cout << "Creating fake bootstrap method" << endl;
    
    VMMethod* bootstrap_method = NewMethod(SymbolForChars("bootstrap"), 1, 0);
    bootstrap_method->SetBytecode(0, BC_HALT);
    bootstrap_method->SetNumberOfLocals(0);
    //bootstrap_method->SetNumberOfArguments(0);
    bootstrap_method->SetMaximumNumberOfStackElements(2);
    bootstrap_method->SetHolder(system_class);
    cout << "Cheer!!! We can start the Interpreter now!" << endl;

    if (argv.size() == 0) 
    {
        Shell* shell = new Shell(bootstrap_method);
        shell->Start();
        return;
    }
    /*char g[10];
    cout << "Press a key, to start Interpreter" << endl;
    cin.getline(g,10);*/

    /* only trace bootstrap if the number of cmd-line "-d"s is > 2 */
    short trace = 2 - dump_bytecodes;
    if(!(trace > 0)) dump_bytecodes = 1;

    VMArray* arguments_array = _UNIVERSE->NewArrayFromArgv(argv);
    
    VMFrame* bootstrap_frame = interpreter->PushNewFrame(bootstrap_method);
    bootstrap_frame->Push(system_object);
    bootstrap_frame->Push((VMObject*)arguments_array);

    VMInvokable* initialize = 
        (VMInvokable*)system_class->LookupInvokable(this->SymbolForChars("initialize:"));
    initialize->Invoke(bootstrap_frame);
    
    // reset "-d" indicator
    if(!(trace>0)) dump_bytecodes = 2 - trace;

    VMObject* r = new(this->heap)VMObject(0);
    VMObject& ref = *arguments_array;
    VMArray& aref = reinterpret_cast<VMArray&>(ref);
    VMString* f = (VMString*)aref[0];
    cout << "aref[0]: " << f->GetChars() << endl;

    interpreter->Start();
}


Universe::~Universe()
{
    if (interpreter) 
        delete(interpreter);
    if (compiler) 
        delete(compiler);
    if (heap) 
        delete(heap);
    if (symboltable) 
        delete(symboltable);
}


VMObject* Universe::NewTaggedInteger(int32_t val)
{
    val += 0x80000000;
    return (VMObject*) val;
}


void Universe::Assert( bool value)
{
    if (!value) 
    {
        cout << "Assertion failed" << endl;
    }

}


VMClass* Universe::GetBlockClass()
{
    return block_class;
}


VMClass* Universe::GetBlockClassWithArgs( int number_of_arguments)
{
    this->Assert(number_of_arguments < 10);

    ostringstream Str;
    Str << "Block" << number_of_arguments ;
    pString block_name(Str.str());
    VMSymbol* name = SymbolFor(block_name);

    if (HasGlobal(name))
        return (VMClass*)GetGlobal(name);

    VMClass* result = LoadClassBasic(name, NULL);

    result->AddInstancePrimitive(new (_HEAP) VMEvaluationPrimitive(number_of_arguments) );

    SetGlobal(name, (VMObject*) result);

    return result;
}



VMObject* Universe::GetGlobal( VMSymbol* name)
{
    if (HasGlobal(name))
        return (VMObject*)globals[name->GetStdString()];

    return NULL;
}


bool Universe::HasGlobal( VMSymbol* name)
{
    if (globals[name->GetStdString()] != NULL) return true;
    else return false;
}


void Universe::InitializeSystemClass( VMClass* system_class, VMClass* super_class, const char* name)
{
    pString s_name(name);

    if (super_class != NULL) 
    {
        system_class->SetSuperClass(super_class);
        VMClass* sys_class_class = system_class->GetClass();
        VMClass* super_class_class = super_class->GetClass();
        sys_class_class->SetSuperClass(super_class_class);
    } else {
        VMClass* sys_class_class = system_class->GetClass();
        sys_class_class->SetSuperClass(class_class);
    }

    VMClass* sys_class_class = system_class->GetClass();

    system_class->SetInstanceFields(NewArray(0));
    sys_class_class->SetInstanceFields(NewArray(0));

    system_class->SetInstanceInvokables(NewArray(0));
    sys_class_class->SetInstanceInvokables(NewArray(0));

    system_class->SetName(SymbolFor(s_name));
    ostringstream Str;
    Str << s_name << " class";
    pString class_class_name(Str.str());
    sys_class_class->SetName(SymbolFor(class_class_name));

    SetGlobal(system_class->GetName(), (VMObject*)system_class);


}


VMClass* Universe::LoadClass( VMSymbol* name)
{
   if (HasGlobal(name))
       return dynamic_cast<VMClass*>(GetGlobal(name));

   VMClass* result = LoadClassBasic(name, NULL);

   if (!result) 
   {
       cout << "can\'t load class " << name->GetStdString() << endl;
       Universe::Quit(ERR_FAIL);
   }

   if (result->HasPrimitives() || result->GetClass()->HasPrimitives())
       result->LoadPrimitives(class_path, cp_count);
    
   return result;
}


VMClass* Universe::LoadClassBasic( VMSymbol* name, VMClass* system_class)
{
    pString s_name = name->GetStdString();
    cout << s_name.c_str() << endl;
    VMClass* result;

    for (vector<pString>::iterator i = class_path.begin();
         i != class_path.end(); ++i)
    {
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


VMClass* Universe::LoadShellClass( pString& stmt)
{
    VMClass* result = compiler->CompileClassString(stmt, NULL);
     if(dump_bytecodes)
         Disassembler::Dump(result);
    return result;
}


void Universe::LoadSystemClass( VMClass* system_class)
{
    VMClass* result =
        LoadClassBasic(system_class->GetName(), system_class);
    pString s = system_class->GetName()->GetStdString();

    if (!result) {
        cout << "Can\'t load system class: " << system_class->GetName()->GetStdString();
        Universe::Quit(ERR_FAIL);
    }

    if (result->HasPrimitives() || result->GetClass()->HasPrimitives())
        result->LoadPrimitives(class_path, cp_count);
}


VMArray* Universe::NewArray( int size)
{
    int additionalBytes = size*sizeof(VMObject*);
    VMArray* result = new (heap, additionalBytes) VMArray(size);
    result->SetClass(array_class);
    return result;
}


VMArray* Universe::NewArrayFromArgv( const vector<pString>& argv)
{
    VMArray* result = NewArray(argv.size());
    int j = 0;
    for (vector<pString>::const_iterator i = argv.begin();
         i != argv.end(); ++i)
    {
        result->SetIndexableField(j, NewString(*i));
        ++j;
    }

    return result;
}


VMArray* Universe::NewArrayList(pList& list )
{
    int size = list.Size();
    VMArray* result = NewArray(size);

    if (result) 
    {
        for (int i = 0; i < size; ++i)
        {
            VMObject* elem = list.get(i);
            result->SetIndexableField(i, elem);
        }
    }
    return result;
}


VMBigInteger* Universe::NewBigInteger( int64_t value)
{
    VMBigInteger* result = new (heap) VMBigInteger(value);
    result->SetClass(biginteger_class);

    return result;
}


VMBlock* Universe::NewBlock( VMMethod* method, VMFrame* context, int arguments)
{
    VMBlock* result = new (heap) VMBlock;
    result->SetClass(this->GetBlockClassWithArgs(arguments));

    result->SetMethod(method);
    result->SetContext(context);

    return result;
}


VMClass* Universe::NewClass( VMClass* class_of_class)
{
    int num_fields = class_of_class->GetNumberOfInstanceFields();
    VMClass* result;
    int additionalBytes = num_fields * sizeof(VMObject*);
    if (num_fields) result = new (heap, additionalBytes) VMClass(num_fields);
    else result = new (heap) VMClass;

    result->SetClass(class_of_class);

    return result;
}


VMDouble* Universe::NewDouble( double value)
{
    VMDouble* result = new (heap) VMDouble(value);
    result->SetClass(double_class);
    return result;
}


VMFrame* Universe::NewFrame( VMFrame* previous_frame, VMMethod* method)
{
    int length = method->GetNumberOfArguments() +
                 method->GetNumberOfLocals()+
                 method->GetMaximumNumberOfStackElements(); 
   
    int additionalBytes = length * sizeof(VMObject*);
    VMFrame* result = new (heap, additionalBytes) VMFrame(length);
    result->SetClass(frame_class);

    result->SetMethod(method);

    if (previous_frame != NULL) 
        result->SetPreviousFrame(previous_frame);

    result->ResetStackPointer();
    result->SetBytecodeIndex(0);

    return result;
}


VMObject* Universe::NewInstance( VMClass*  class_of_instance)
{
    //the number of fields for allocation. We have to calculate the clazz
    //field out of this, because it is already taken care of by VMObject
    int num_of_fields = class_of_instance->GetNumberOfInstanceFields() - 1;
    //the additional space needed is calculated from the number of fields
    int additionalBytes = num_of_fields * sizeof(VMObject*);
    VMObject* result = new (heap, additionalBytes) VMObject(num_of_fields);
    result->SetClass(class_of_instance);
    return result;
}

VMInteger* Universe::NewInteger( int32_t value)
{
    VMInteger* result = new (heap) VMInteger(value);
    result->SetClass(integer_class);
    return result;
}

VMClass* Universe::NewMetaclassClass()
{
    VMClass* result = new (heap) VMClass;
    result->SetClass(new (heap) VMClass);

    VMClass* mclass = result->GetClass();
    mclass->SetClass(result);

    return result;
}


VMMethod* Universe::NewMethod( VMSymbol* signature, 
                    size_t number_of_bytecodes, size_t number_of_constants)
{
    //Method needs space for the bytecodes and the pointers to the constants
    int additionalBytes = number_of_bytecodes + 
                number_of_constants*sizeof(VMObject*);
    VMMethod* result = new (heap,additionalBytes) 
                VMMethod(number_of_bytecodes, number_of_constants);
    result->SetClass(method_class);

    result->SetSignature(signature);

    return result;
}


VMString* Universe::NewString( const pString& str)
{
    //string needs space for str.length characters plus one byte for '\0'
    int additionalBytes = str.length() + 1;
    VMString* result = new (heap, additionalBytes) VMString(str);
    result->SetClass(string_class);

    return result;
}


VMSymbol* Universe::NewSymbol( const pString& str )
{
    //symbol needs space for str.length characters plus one byte for '\0'
    int additionalBytes = str.length() + 1;
    VMSymbol* result = new (heap, additionalBytes) VMSymbol(str);
    result->SetClass(symbol_class);

    symboltable->insert(result);

    return result;
}


VMClass* Universe::NewSystemClass()
{
    VMClass* system_class = new (heap) VMClass();

    system_class->SetClass(new (heap) VMClass());
    VMClass* mclass = system_class->GetClass();
    
    mclass->SetClass(metaclass_class);

    return system_class;
}


VMSymbol* Universe::SymbolFor( const pString& str)
{
    
    VMSymbol* result = symboltable->lookup(str);
    
    return (result != NULL) ?
           result :
           NewSymbol(str);
}


VMSymbol* Universe::SymbolForChars( const char* str)
{
    return SymbolFor(std::string(str));
}


void Universe::SetGlobal(VMSymbol* name, VMObject *val)
{
    pString str =  name->GetStdString();

    globals[str] = val;
	//globals.insert(pair<pString, VMObject*>(name->GetStdString(), val));
    VMObject* p = globals[str];
    if (p == NULL)
    {
        cout << "Global " << str << " just added, but globals[" << str << "] == NULL" << endl;
    }
}
