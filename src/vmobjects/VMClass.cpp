#include "VMClass.h"
#include "VMArray.h"
#include "VMSymbol.h"
#include "VMInvokable.h"
#include "VMPrimitive.h"
#include <fstream>
#include <typeinfo>

/*
 * Format definitions for Primitive naming scheme.
 *
 */
#define CLASS_METHOD_FORMAT_S "%s::%s"
// as in AClass::aClassMethod
#define INSTANCE_METHOD_FORMAT_S "%s::%s_"
// as in AClass::anInstanceMethod_


VMClass::VMClass() : VMObject(4)
{
    //this->objectSize = sizeof(VMClass);
}

VMClass::VMClass( int number_of_fields ) : VMObject(number_of_fields + 4)
{
    //this->objectSize = sizeof(VMClass) + number_of_fields*sizeof(VMObject*);
}


bool VMClass::has_super_class()
{
    return (super_class != NULL && super_class != nil_object);
}


bool VMClass::add_instance_invokable(VMObject *ptr)
{
    VMInvokable* newInvokable = dynamic_cast<VMInvokable*>(ptr);
    if (newInvokable == NULL) {
        cout << "Error: trying to add non-invokable to invokables array" << endl;
        throw std::bad_typeid();//("Trying to add non-invokable to invokables array");
    }
	for (int i = 0; i < instance_invokables->GetNumberOfIndexableFields(); ++i)
	{
        VMInvokable* inv = dynamic_cast<VMInvokable*>( instance_invokables->GetIndexableField(i) );
		if (inv != 0) {
            if (newInvokable->get_signature() == inv->get_signature())
            {
                this->set_instance_invokable(i, ptr);
                return false;
            }
			//if (ptr->GetSignature() == ((VMInvokable*)cmp)->GetSignature()) {
			//	  instance_invokables->SetIndexableField(i, ptr);
			//	  return false;
			//}
        } else {
            cout << "Invokables array corrupted. Either NULL pointer added or pointer to non-invokable." << endl;
            throw std::bad_typeid();//"Invokables array corrupted. Either NULL pointer added or pointer to non-invokable.");
        }
	}
	if (instance_invokables->GetNumberOfIndexableFields() >= instance_invokables->GetArraySize())
		instance_invokables = instance_invokables->CopyAndExtendWith(ptr);
	else instance_invokables->SetIndexableField(ptr);

	return true;
}

void VMClass::add_instance_primitive(VMPrimitive *ptr)
{
	if (add_instance_invokable((VMObject*)ptr)) {
		//cout << "Warn: Primitive "<<ptr->GetSignature<<" is not in class definition for class " << name->GetStdString() << endl;
	}
}

VMSymbol* VMClass::get_instance_field_name(int index)
{
	if (index >= numberOfSuperInstanceFields())
	{
		index -= numberOfSuperInstanceFields();
		return (VMSymbol*) instance_fields->GetIndexableField(index);
	}
	
	return super_class->get_instance_field_name(index);
}

VMClass* VMClass::get_super_class()
{
	return super_class;
}

void VMClass::set_super_class(VMClass* sup)
{
	super_class = sup;
}

VMSymbol* VMClass::get_name()
{
	return name;
}
void VMClass::set_name(VMSymbol* nam)
{
	name = nam;
}

VMArray* VMClass::get_instance_fields()
{
	return instance_fields;
}


void VMClass::set_instance_fields(VMArray* inst_fields)
{
	instance_fields = inst_fields;
}

VMArray  *VMClass::get_instance_invokables()
{
	return instance_invokables;
}

void      VMClass::set_instance_invokables(VMArray* invokables)
{
	instance_invokables = invokables;
}

int       VMClass::get_number_of_instance_invokables()
{
	return instance_invokables->GetNumberOfIndexableFields();
}

VMObject *VMClass::get_instance_invokable(int index)
{
    return instance_invokables->GetIndexableField(index);
	//return instance_invokables[index];
	//return NULL;
}

void      VMClass::set_instance_invokable(int index, VMObject* invokable)
{
	instance_invokables->SetIndexableField(index, invokable);
	//instance_invokables[index] = invokable;
}

VMObject* VMClass::lookup_invokable(VMSymbol* name)
{
    VMInvokable* invokable = NULL;
    for (int i = 0; i < get_number_of_instance_invokables(); ++i)
    {
        invokable = (VMInvokable*)(get_instance_invokable(i));
        if (invokable->get_signature() == name) 
            return (VMObject*)invokable;
    }
    //look in super class
    if (this->has_super_class()) 
    {
        invokable = (VMInvokable*)this->super_class->lookup_invokable(name);
    }
	return (VMObject*)invokable;
}

int       VMClass::lookup_field_index(VMSymbol* name)
{
    for (int i = 0; i <=get_number_of_instance_fields(); ++i) //even with get_number_of_instance_fields == 0 there is the class field
    {
        if (name == this->get_instance_field_name(i) ||
            name->GetStdString() == this->get_instance_field_name(i)->GetStdString()) 
                return i;
    }
	return -1;
}


int       VMClass::get_number_of_instance_fields()
{
	return instance_fields->GetNumberOfIndexableFields()
           + this->numberOfSuperInstanceFields();
}

bool      VMClass::has_primitives()
{
	for (int i = 0; i < get_number_of_instance_invokables(); ++i)
    {
        VMInvokable* invokable = (VMInvokable*)(get_instance_invokable(i));
        if (invokable->is_primitive()) return true;
    }
    return false;
}


void      VMClass::load_primitives(const vector<pString>& cp,int cp_count)
{//todo do this the "right" way. for now we have a fake dll lookup in Core.h

    //// the library handle
    ifstream* dlhandle = NULL;
    //void* dlhandle=NULL;
    //
    //// cached object properties
    pString cname = this->name->GetStdString();
    //pString cname = this->name->GetStdString;
/*
    //// iterate the classpathes
    for(vector<pString>::const_iterator i = cp.begin(); (i != cp.end()) && dlhandle == NULL; ++i) {
    //    //
    //    // check the core library
    //    //
    //    
    //    
        pString loadstring = gen_core_loadstring(*i);
        dlhandle = load_lib(loadstring);
        //SEND(loadstring, free);
        if(dlhandle != NULL && dlhandle->good() && is_responsible(dlhandle, cname))
    //        //
    //        // the core library is found and responsible
    //        //
              break;
        else if (dlhandle != NULL) {
            dlhandle->close();
            delete(dlhandle);
            dlhandle = NULL;
        }
    //    
    //    
    //    //
    //    // the core library is not found or responsible, 
    //    // continue w/ class file
    //    //
    //    
    //    
        loadstring = gen_loadstring(*i, cname);
        dlhandle = load_lib(loadstring);
    //    SEND(loadstring, free);
        if(dlhandle != NULL && dlhandle->good()) {
    //        //
    //        // the class library was found...
    //        //
            if(is_responsible(dlhandle, cname)) {
    //            //
    //            // ...and is responsible.
    //            //
                break;
            } else {
    //            //
    //            // ... but says not responsible, but have to
    //            // close it
    //            //
                dlhandle->close();
                delete(dlhandle);
                _UNIVERSE->error_exit("Library claims no resonsibility, but musn't!");
            }
            dlhandle->close();
            delete(dlhandle);
            dlhandle = NULL;
        }
    //    /*
    //     * continue checking the next class path
    //     *
    //     
    }

    ////
    //// finished cycling,
    //// check if a lib was found.
    ////
    if(!(dlhandle != NULL)) {
        cout << "load failure: ";
        cout << "could not load primitive library for " << cname << endl;
        _UNIVERSE->quit(ERR_FAIL);
    }

    ///*
    // * do the actual loading for both class and metaclass
    // *
    // */



    set_primitives(this, dlhandle, cname, INSTANCE_METHOD_FORMAT_S);
    set_primitives(this->GetClass(), dlhandle, cname, CLASS_METHOD_FORMAT_S);
}


void VMClass::MarkReferences()
{
	VMObject::MarkReferences();
	super_class->MarkReferences();
	name->MarkReferences();
	instance_fields->MarkReferences();
	instance_invokables->MarkReferences();
}

//private Methods

int VMClass::numberOfSuperInstanceFields()
{
	if (this->has_super_class()) return this->super_class->get_number_of_instance_fields();
	return 0;
}

//load_primitives helper
#define shared_extension ".csp"
pString VMClass::gen_loadstring(const pString& cp, 
                       const pString& cname
                       ) {
    
    pString loadstring = string(cp);
    loadstring += file_separator;
    loadstring += cname;
    loadstring += shared_extension;

    return loadstring;
}


/**
 *  generate the string containing the path to a SOMCore which may be located
 *  at the classpath given.
 *
 */
pString VMClass::gen_core_loadstring(const pString& cp) {
    #define S_CORE "SOMCore"
    pString corename = string(S_CORE);
    pString result = gen_loadstring(cp, corename);
    //SEND(corename, free);
    
    return result;
}


/**
 * load the given library, return the handle
 *
 */
ifstream* VMClass::load_lib(const pString& path) {
    cout << "load_lib " << path << endl;
    //#if !defined(CSOM_WIN)
    //    #ifdef DEBUG
    //        #define    DL_LOADMODE RTLD_NOW
    //    #else
    //        #define    DL_LOADMODE RTLD_LAZY
    //    #endif DEBUG
    //#endif
    
    // static handle. will be returned
    //ifstream* fs = new ifstream();

    //fs->open(path.c_str(), std::ios_base::in);
    
	// try load lib
	/*if((handle=dlopen(SEND(path, chars), DL_LOADMODE)))
		//found.
		return handle;
	else
        return NULL;*/
    return NULL;
}


/**
 * check, whether the lib referenced by the handle supports the class given
 *
 */
bool VMClass::is_responsible(ifstream* handle, const pString& cl) {

/*    // function handler
    supports_class_fn supports_class=NULL;

    supports_class = (supports_class_fn)dlsym(handle, "supports_class");

	if(!supports_class) {
        debug_error(dlerror());
        Universe_error_exit("Library doesn't have expected format");
    }
    
    // test class responsibility
    return (*supports_class)(SEND(class, chars));*/
    return true;
}




/*
 * set the routines for primitive marked invokables of the given class
 *
 */
void VMClass::set_primitives(VMClass* cl, ifstream* handle, const pString& cname,
                    const char* format
                    ) {    
    VMPrimitive* the_primitive;
    PrimitiveRoutine*   routine=NULL;
    VMInvokable* an_invokable;
    // iterate invokables
    for(int i = 0; i < cl->get_number_of_instance_invokables(); i++) {
        
        an_invokable = (VMInvokable*)cl->get_instance_invokable(i);
        cout << "cname: >" << cname << "<"<< endl;
        cout << an_invokable->get_signature()->GetStdString() << endl;
        if(an_invokable->is_primitive()) {
            the_primitive = (VMPrimitive*) an_invokable;
            //
            // we have a primitive to load
            // get it's selector
            //
            VMSymbol* sig =  the_primitive->get_signature();

            pString selector = sig->GetPlainString();
            
   //         pString symbol = pString(cname);
   //         symbol += "
   //         symbol += selector;

			//{ //string block          
			//	char symbol[SEND(cname, length) + SEND(selector, length)+2 + 1];
   //                                                             //2 for 2x '_'
			//	sprintf(symbol, format,
			//		SEND(cname, chars),
			//		SEND(selector, chars));
   //              
			//	// try loading the primitive
			//	routine = (routine_fn)dlsym(handle, symbol);
   //         }

            routine = Core::create(cname, selector);
            
            if(!routine) {
                cout << "could not load primitive '"<< selector <<"' for class " << cname << endl;
                            
                _UNIVERSE->quit(ERR_FAIL);
            }
            
            // set routine
            the_primitive->SetRoutine(routine);
            the_primitive->SetEmpty(false);
        }
    }
}

