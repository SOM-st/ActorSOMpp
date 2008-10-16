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
#include "../compiler/SourcecodeCompiler.h"
#include "../vmobjects/Symboltable.h"
#include "../interpreter/bytecodes.h"
#include <sstream> 
#include <string.h>
#include <stdlib.h>
#include "../compiler/Disassembler.h"
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
    if (!theUniverse) {
        error_exit("Trying to access uninitialized Universe, exiting.");
    }
	return theUniverse;
}

Universe* Universe::operator->()
{
    if (!theUniverse) {
        error_exit("Trying to access uninitialized Universe, exiting.");
    }
	return theUniverse;
}

vector<pString> Universe::handle_arguments( int argc, char** argv )
{

    vector<pString> vm_args = vector<pString>();
    dump_bytecodes = 0;
    gc_verbosity = 0;
    for (int i = 1; i < argc ; ++i)
    {
        if (strncmp(argv[i], "-cp", 3) == 0) 
        {
            if ((argc == i + 1) || class_path.size() > 0)
                print_usage_and_exit(argv[0]);
            setup_class_path(pString(argv[++i]));
        } else if (strncmp(argv[i], "-d", 2) == 0) {
            dump_bytecodes++;
        } else if (strncmp(argv[i], "-g", 2) == 0) {
            gc_verbosity++;
        } else if (argv[i][0] == '-' && argv[i][1] == 'H') {
            int heap_size = atoi(argv[i] + 2);
            heapSize = heap_size;
        } else if ((strncmp(argv[i], "-h", 2) == 0) ||
            (strncmp(argv[i], "--help", 6) == 0)) {
                print_usage_and_exit(argv[0]);
        } else {
            vector<pString> ext_path_tokens = vector<pString>(2);
            pString tmp_string = pString(argv[i]);
            if (this->get_path_class_ext(ext_path_tokens, tmp_string) == ERR_SUCCESS)
            {
                this->add_class_path(ext_path_tokens[0]);
                vm_args.push_back(ext_path_tokens[1]);
                //delete(&ext_path_tokens);
            } else {
                vm_args.push_back(pString(tmp_string));
            }
            //delete(&tmp_string);
            //delete(&ext_path_tokens);
        }
    }
    add_class_path(pString("."));

    return vm_args;
}


void Universe::start(int argc, char** argv)
{
    /*int vm_argc = 0;
    
    vector<pString> vm_argv = handle_arguments(&vm_argc, argc, argv);*/
    Core::setup();
	theUniverse = new Universe();
    theUniverse->initialize(argc, argv);
}

void Universe::quit(int err)
{
    if (theUniverse) delete(theUniverse);
    
    Core::tearDown();
    exit(err);
}

void Universe::error_exit( const char* err)
{
    cout << "Runtime error: " << err << endl;
    quit(ERR_FAIL);
}

int Universe::get_path_class_ext(vector<pString>& tokens,const pString& arg )
{
#define EXT_TOKENS 2
    

    int fp_index = arg.find_last_of(file_separator);
    int ssep_index = arg.find_last_of('.');

    if (fp_index < 0) 
    { //no new path
        return ERR_FAIL;
    } else tokens[0] = arg.substr(0, fp_index);

    ssep_index = ( (ssep_index >= 0) && (ssep_index > fp_index)) ?
                 (ssep_index - 1) :
                 arg.length();

    tokens[1] = arg.substr(fp_index + 1, ssep_index);

    return ERR_SUCCESS;
}

int Universe::setup_class_path( const pString& cp )
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

int Universe::add_class_path( const pString& cp )
{
    class_path.push_back(cp);
    ++cp_count;
    return ERR_SUCCESS;
}

void Universe::print_usage_and_exit( char* executable )
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

    quit(ERR_SUCCESS);
}

Universe::Universe(){};

void Universe::prepareNilObject()
{
    nil_object->SetField(0, nil_object);
}

void Universe::initialize(int _argc, char** _argv)
{
    heapSize = 1048576;

    vector<pString> argv = this->handle_arguments(_argc, _argv);

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
    metaclass_class = this->new_metaclass_class();
    cout << "Metaclass Class created, creating System classes" << endl;
    object_class    = this->new_system_class();
    nil_class       = this->new_system_class();
    class_class     = this->new_system_class();
    array_class     = this->new_system_class();
    symbol_class    = this->new_system_class();
    method_class    = this->new_system_class();
    integer_class   = this->new_system_class();
    biginteger_class= this->new_system_class();
    frame_class     = this->new_system_class();
    primitive_class = this->new_system_class();
    string_class    = this->new_system_class();
    double_class    = this->new_system_class();
    cout << "System classes created" << endl;
    
    nil_object->SetClass(nil_class);

    cout << "Initialize System Classes" << endl;
    this->initialize_system_class(object_class, NULL, "Object");
    this->initialize_system_class(class_class, object_class, "Class");
    this->initialize_system_class(metaclass_class, class_class, "Metaclass");
    this->initialize_system_class(nil_class, object_class, "Nil");
    this->initialize_system_class(array_class, object_class, "Array");
    this->initialize_system_class(method_class, array_class, "Method");
    this->initialize_system_class(symbol_class, object_class, "Symbol");
    this->initialize_system_class(integer_class, object_class, "Integer");
    this->initialize_system_class(biginteger_class, object_class,
                                     "BigInteger");
    this->initialize_system_class(frame_class, array_class, "Frame");
    this->initialize_system_class(primitive_class, object_class,
                                     "Primitive");
    this->initialize_system_class(string_class, object_class, "String");
    this->initialize_system_class(double_class, object_class, "Double");

    cout << "System classes initialized, now let's load them!"
            << endl;

    this->load_system_class(object_class);
    this->load_system_class(class_class);
    this->load_system_class(metaclass_class);
    this->load_system_class(nil_class);
    this->load_system_class(array_class);
    this->load_system_class(method_class);
    this->load_system_class(symbol_class);
    this->load_system_class(integer_class);
    this->load_system_class(biginteger_class);
    this->load_system_class(frame_class);
    this->load_system_class(primitive_class);
    this->load_system_class(string_class);
    this->load_system_class(double_class);

    cout << "YAY, the system classes are loaded" << endl;
    
    cout << "loading block class" << endl;
    block_class = load_class(symbol_for_chars("Block"));
    
    cout << "setting up true and false" << endl;
    true_object = new_instance(load_class(symbol_for_chars("True")));
    false_object = new_instance(load_class(symbol_for_chars("False")));

    cout << "load System" << endl;
    system_class = load_class(symbol_for_chars("System"));
    VMObject* system_object = new_instance(system_class);

    cout << "Set globals" << endl;
    this->set_global(symbol_for_chars("nil"), nil_object);
    this->set_global(symbol_for_chars("true"), true_object);
    this->set_global(symbol_for_chars("false"), false_object);
    this->set_global(symbol_for_chars("system"), system_object);
    this->set_global(symbol_for_chars("System"), system_class);
    this->set_global(symbol_for_chars("Block"), block_class);

    cout << "Creating fake bootstrap method" << endl;
    
    VMMethod* bootstrap_method = new_method(symbol_for_chars("bootstrap"), 1, 0);
    bootstrap_method->SetBytecode(0, BC_HALT);
    bootstrap_method->SetNumberOfLocals(0);
    //bootstrap_method->SetNumberOfArguments(0);
    bootstrap_method->SetMaximumNumberOfStackElements(2);
    bootstrap_method->SetHolder(system_class);
    cout << "Cheer!!! We can start the Interpreter now!" << endl;

    if (argv.size() == 0) {
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

    VMArray* arguments_array = _UNIVERSE->new_array_from_argv(argv);
    
    VMFrame* bootstrap_frame = interpreter->PushNewFrame(bootstrap_method);
    bootstrap_frame->Push(system_object);
    bootstrap_frame->Push((VMObject*)arguments_array);

    VMInvokable* initialize = 
        (VMInvokable*)system_class->LookupInvokable(this->symbol_for_chars("initialize:"));
    initialize->Invoke(bootstrap_frame);
    
    // reset "-d" indicator
    if(!(trace>0)) dump_bytecodes = 2 - trace;

    interpreter->Start();

    //-----------------

 //   
	////while (1) {
	//	VMObject *vmo = new (heap) VMObject;
	//	cout << "vmo (VMObject) Obj size:" << vmo->GetObjectSize() << endl;
	//	//cout << sizeof(*vmo) << endl;
	//	VMObject *vmo2 = new (heap, 0) VMMethod(0,0);
	//	cout << "vmo2 (VMMethod) Obj size:" << vmo2->GetObjectSize() << endl;
	//	//cout << sizeof(*vmo2) << endl;
	////}
	////vector<VMObject*, HeapAllocator<VMObject*> > v = vector<VMObject*, HeapAllocator<VMObject*> >(HeapAllocator<VMObject*>(heap));
	////v.push_back(vmo);
	////v.push_back(vmo2);

	//VMString *vmstr = new (heap) VMString;
	//cout << "vmstr Obj size:" << vmstr->GetObjectSize() << endl;
	//VMString *vmstr2 = new (heap, strlen("Stringtest")+1 ) VMString("Stringtest");
	//cout << "vmstr2 Obj size:" << vmstr2->GetObjectSize() << endl;
	//cout << vmstr2->GetChars() << endl;

	//VMArray* vma = new (heap, 4*sizeof(VMObject*)) VMArray(4);
	//vma->SetIndexableField(0, vmo);
	//vma->SetIndexableField(1, vmo2);
	//vma->SetIndexableField(2, vmstr);
	//vma->SetIndexableField(3, vmstr2);
	//cout << "sizeof(VMArray)" << sizeof(VMArray) << endl;
	//cout << "vma Obj size:" << vma->GetObjectSize() << endl;
	//cout << "vma array size:" << vma->GetArraySize() << endl;

 //   VMInvokable* inv = dynamic_cast<VMInvokable*>(vmo2);

 //   if (inv != NULL) cout << "inv is invokable" << endl;

 //   inv = dynamic_cast<VMInvokable*>(vmo);
 //   if (inv == NULL) cout << "inv is not invokable" << endl;

 //   //if (typeid(vmo2).name() == VMObject) cout << "yeah" << endl;
 //   //cout << typeid(vmo2).name() << endl;
 //   //cout << typeid(vmo2) << endl;
	////vmstr->FromCString("VMString Test");
	////cout << vmstr->ToCString() << endl;
	////cout << vmo->GetObjectSize() << endl;
	////cout << vmo2->GetObjectSize() << endl;
	//
	////this->RunGC();

	///*ExtendedList *list = new ExtendedList();
	//list->Add(vmo);
	//list->Add(vmo2);
	//cout << "List-size:" << list->Size() << endl;*/
	//
	////if (argc < 2) {
	////	cout << "Please specify the file(s) you'd like to parse" << endl;
	////	cout << "Usage: cppsom [files]" << endl;
	////	return -1;
	////}
	////
	////for (int i = 1; i < argc; i++) { 
	////	ifstream fp;
	////	fp.open(argv[i], std::ios_base::in);
	////	if (!fp.is_open()) {
	////		cout << "error opening " << argv[i] <<endl;
	////		continue;
	////	}
	////	//int cdc = 1;
	////	class_generation_context cdc;
	////	cout << "starting " << argv[i] << endl;
	////	Parser* p = new Parser(fp);
	////	p->Classdef(&cdc);

	////	//clean up
	////	fp.close();
	////	delete(p);
	////	
	////	cout << "finished " << argv[i] << endl;

	////}
 //   
 //   //---------------------
}

Universe::~Universe()
{
    if (interpreter) 
        delete(interpreter);
    if (compiler) 
        delete(compiler);
    if (heap) {
        cout << "Heap alloc count: " << heap->allocCount << endl;
        delete(heap);
    }
    if (symboltable) 
        delete(symboltable);
}

VMObject* Universe::new_tagged_integer(int32_t val)
{
    val += 0x80000000;
    return (VMObject*) val;
}

void Universe::Assert( bool value)
{
    if (!value) {
        cout << "Assertion failed" << endl;
    }

}


VMClass* Universe::get_block_class()
{
    return block_class;
}

VMClass* Universe::get_block_class_with_args( int number_of_arguments)
{
    this->Assert(number_of_arguments < 10);

    ostringstream Str;
    Str << "Block" << number_of_arguments ;
    pString block_name(Str.str());
    VMSymbol* name = symbol_for(block_name);

    if (has_global(name))
        return (VMClass*)get_global(name);

    VMClass* result = load_class_basic(name, NULL);

    result->AddInstancePrimitive(new (_HEAP) VMEvaluationPrimitive(number_of_arguments) );

    set_global(name, (VMObject*) result);

    return result;
}



VMObject* Universe::get_global( VMSymbol* name)
{
    if (has_global(name))
        return (VMObject*)globals[name->GetStdString()];

    return NULL;
}


bool Universe::has_global( VMSymbol* name)
{
    if (globals[name->GetStdString()] != NULL) return true;
    else return false;
}

void Universe::initialize_system_class( VMClass* system_class, VMClass* super_class, const char* name)
{
    pString s_name(name);

    if (super_class != NULL) {
        system_class->SetSuperClass(super_class);
        VMClass* sys_class_class = system_class->GetClass();
        VMClass* super_class_class = super_class->GetClass();
        sys_class_class->SetSuperClass(super_class_class);
    } else {
        VMClass* sys_class_class = system_class->GetClass();
        sys_class_class->SetSuperClass(class_class);
    }

    VMClass* sys_class_class = system_class->GetClass();

    system_class->SetInstanceFields(new_array(0));
    sys_class_class->SetInstanceFields(new_array(0));

    system_class->SetInstanceInvokables(new_array(0));
    sys_class_class->SetInstanceInvokables(new_array(0));

    system_class->SetName(symbol_for(s_name));
    ostringstream Str;
    Str << s_name << " class";
    pString class_class_name(Str.str());
    sys_class_class->SetName(symbol_for(class_class_name));

    set_global(system_class->GetName(), (VMObject*)system_class);


}

VMClass* Universe::load_class( VMSymbol* name)
{
   if (has_global(name))
       return dynamic_cast<VMClass*>(get_global(name));

   VMClass* result = load_class_basic(name, NULL);

   if (!result) {
       cout << "can\'t load class " << name->GetStdString() << endl;
       Universe::quit(ERR_FAIL);
   }

   if (result->HasPrimitives() || result->GetClass()->HasPrimitives())
       result->LoadPrimitives(class_path, cp_count);
    
   return result;
}

VMClass* Universe::load_class_basic( VMSymbol* name, VMClass* system_class)
{
    pString s_name = name->GetStdString();
    VMClass* result;

    for (vector<pString>::iterator i = class_path.begin();
         i != class_path.end(); ++i)
    {
        result = compiler->compile_class(*i, name->GetStdString(), system_class);
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

VMClass* Universe::load_shell_class( pString& stmt)
{
    VMClass* result = compiler->compile_class_string(stmt, NULL);
     if(dump_bytecodes)
         Disassembler::Dump(result);
    return result;
}

void Universe::load_system_class( VMClass* system_class)
{
    VMClass* result =
        load_class_basic(system_class->GetName(), system_class);
    pString s = system_class->GetName()->GetStdString();

    if (!result) {
        cout << "Can\'t load system class: " << system_class->GetName()->GetStdString();
        Universe::quit(ERR_FAIL);
    }

    if (result->HasPrimitives() || result->GetClass()->HasPrimitives())
        result->LoadPrimitives(class_path, cp_count);
}

VMArray* Universe::new_array( int size)
{
    int additionalBytes = size*sizeof(VMObject*);
    VMArray* result = new (heap, additionalBytes) VMArray(size);
    result->SetClass(array_class);
    return result;
}

VMArray* Universe::new_array_from_argv( const vector<pString>& argv)
{
    VMArray* result = new_array(argv.size());
    int j = 0;
    for (vector<pString>::const_iterator i = argv.begin();
         i != argv.end(); ++i)
    {
        result->SetIndexableField(j, new_string(*i));
        ++j;
    }

    return result;
}

VMArray* Universe::new_array_list(pList& list )
{
    int size = list.Size();
    VMArray* result = new_array(size);

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

VMBigInteger* Universe::new_biginteger( int64_t value)
{
    VMBigInteger* result = new (heap) VMBigInteger(value);
    result->SetClass(biginteger_class);

    return result;
}

VMBlock* Universe::new_block( VMMethod* method, VMFrame* context, int arguments)
{
    VMBlock* result = new (heap) VMBlock;
    result->SetClass(this->get_block_class_with_args(arguments));

    result->SetMethod(method);
    result->SetContext(context);

    return result;
}

VMClass* Universe::new_class( VMClass* class_of_class)
{
    int num_fields = class_of_class->GetNumberOfInstanceFields();
    VMClass* result;
    int additionalBytes = num_fields * sizeof(VMObject*);
    if (num_fields) result = new (heap, additionalBytes) VMClass(num_fields);
    else result = new (heap) VMClass;

    result->SetClass(class_of_class);

    return result;
}

VMDouble* Universe::new_double( double value)
{
    VMDouble* result = new (heap) VMDouble(value);
    result->SetClass(double_class);
    return result;
}

VMFrame* Universe::new_frame( VMFrame* previous_frame, VMMethod* method)
{
    int length = method->GetNumberOfArguments() +
                 method->GetNumberOfLocals()+
                 method->GetMaximumNumberOfStackElements()+1;

    int additionalBytes = length * sizeof(VMObject*);
    VMFrame* result = new (heap, additionalBytes) VMFrame(length);
    result->SetClass(frame_class);

    result->SetMethod(method);

    if (previous_frame != NULL) result->SetPreviousFrame(previous_frame);

    result->ResetStackPointer();
    result->SetBytecodeIndex(0);

    return result;
}

VMObject* Universe::new_instance( VMClass*  class_of_instance)
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

VMInteger* Universe::new_integer( int32_t value)
{
    VMInteger* result = new (heap) VMInteger(value);
    result->SetClass(integer_class);
    return result;
}

VMClass* Universe::new_metaclass_class()
{
    VMClass* result = new (heap) VMClass;
    result->SetClass(new (heap) VMClass);

    VMClass* mclass = result->GetClass();
    mclass->SetClass(result);

    return result;
}

VMMethod* Universe::new_method( VMSymbol* signature, 
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

VMString* Universe::new_string( const pString& str)
{
    //string needs space for str.length characters plus one byte for '\0'
    int additionalBytes = str.length() + 1;
    VMString* result = new (heap, additionalBytes) VMString(str);
    result->SetClass(string_class);

    return result;
}

VMSymbol* Universe::new_symbol( const pString& str )
{
    //symbol needs space for str.length characters plus one byte for '\0'
    int additionalBytes = str.length() + 1;
    VMSymbol* result = new (heap, additionalBytes) VMSymbol(str);
    result->SetClass(symbol_class);

    symboltable->insert(result);

    return result;
}

VMClass* Universe::new_system_class()
{
    VMClass* system_class = new (heap) VMClass();

    system_class->SetClass(new (heap) VMClass());
    VMClass* mclass = system_class->GetClass();
    
    mclass->SetClass(metaclass_class);

    return system_class;
}

VMSymbol* Universe::symbol_for( const pString& str)
{
    
    VMSymbol* result = symboltable->lookup(str);
    
    return (result != NULL) ?
           result :
           new_symbol(str);
}

VMSymbol* Universe::symbol_for_chars( const char* str)
{
    return symbol_for(std::string(str));
}


void Universe::RunGC()
{
	heap->GCCollect();
}

void Universe::set_global(VMSymbol* name, VMObject *val)
{
    pString str =  name->GetStdString();

    globals[str] = val;
	//globals.insert(pair<pString, VMObject*>(name->GetStdString(), val));
    VMObject* p = globals[str];
    if (p == NULL){
        cout << "Global " << str << " just added, but globals[" << str << "] == NULL" << endl;
    }
}
