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


bool VMClass::HasSuperClass()
{
    return (super_class != NULL && super_class != nil_object);
}


bool VMClass::AddInstanceInvokable(VMObject *ptr)
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
            if (newInvokable->GetSignature() == inv->GetSignature())
            {
                this->SetInstanceInvokable(i, ptr);
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
	//if (instance_invokables->GetNumberOfIndexableFields() >= instance_invokables->GetArraySize())
		instance_invokables = instance_invokables->CopyAndExtendWith(ptr);
	//else instance_invokables->SetIndexableField(ptr);

	return true;
}

void VMClass::AddInstancePrimitive(VMPrimitive *ptr)
{
	if (AddInstanceInvokable((VMObject*)ptr)) {
		//cout << "Warn: Primitive "<<ptr->GetSignature<<" is not in class definition for class " << name->GetStdString() << endl;
	}
}

VMSymbol* VMClass::GetInstanceFieldName(int index)
{
	if (index >= numberOfSuperInstanceFields())
	{
		index -= numberOfSuperInstanceFields();
		return (VMSymbol*) instance_fields->GetIndexableField(index);
	}
	
	return super_class->GetInstanceFieldName(index);
}

VMClass* VMClass::GetSuperClass()
{
	return super_class;
}

void VMClass::SetSuperClass(VMClass* sup)
{
	super_class = sup;
}

VMSymbol* VMClass::GetName()
{
	return name;
}
void VMClass::SetName(VMSymbol* nam)
{
	name = nam;
}

VMArray* VMClass::GetInstanceFields()
{
	return instance_fields;
}


void VMClass::SetInstanceFields(VMArray* inst_fields)
{
	instance_fields = inst_fields;
}

VMArray  *VMClass::GetInstanceInvokables()
{
	return instance_invokables;
}

void      VMClass::SetInstanceInvokables(VMArray* invokables)
{

	instance_invokables = invokables;

    for (int i = 0; i < this->GetNumberOfInstanceInvokables(); ++i)
    {
        VMInvokable* inv = (VMInvokable*)instance_invokables->GetIndexableField(i);
        if (inv != nil_object) 
        {
            inv->SetHolder(this);
        }
    }

}

int       VMClass::GetNumberOfInstanceInvokables()
{
	return instance_invokables->GetNumberOfIndexableFields();
}

VMObject *VMClass::GetInstanceInvokable(int index)
{
    return instance_invokables->GetIndexableField(index);
	//return instance_invokables[index];
	//return NULL;
}

void      VMClass::SetInstanceInvokable(int index, VMObject* invokable)
{
	instance_invokables->SetIndexableField(index, invokable);
    if (invokable != nil_object)
    {
        VMInvokable* inv = (VMInvokable*) invokable;
        inv->SetHolder(this);
    }
	//instance_invokables[index] = invokable;
}

VMObject* VMClass::LookupInvokable(VMSymbol* name)
{
    VMInvokable* invokable = NULL;
    for (int i = 0; i < GetNumberOfInstanceInvokables(); ++i)
    {
        invokable = (VMInvokable*)(GetInstanceInvokable(i));
        if (invokable->GetSignature() == name) 
            return (VMObject*)invokable;
    }
    invokable = NULL;
    //look in super class
    if (this->HasSuperClass()) 
    {
        invokable = (VMInvokable*)this->super_class->LookupInvokable(name);
    }
	return (VMObject*)invokable;
}

int       VMClass::LookupFieldIndex(VMSymbol* name)
{
    for (int i = 0; i <=GetNumberOfInstanceFields(); ++i) //even with GetNumberOfInstanceFields == 0 there is the class field
    {
        if (name == this->GetInstanceFieldName(i) ||
            name->GetStdString() == this->GetInstanceFieldName(i)->GetStdString()) 
                return i;
    }
	return -1;
}


int       VMClass::GetNumberOfInstanceFields()
{
	return instance_fields->GetNumberOfIndexableFields()
           + this->numberOfSuperInstanceFields();
}

bool      VMClass::HasPrimitives()
{
	for (int i = 0; i < GetNumberOfInstanceInvokables(); ++i)
    {
        VMInvokable* invokable = (VMInvokable*)(GetInstanceInvokable(i));
        if (invokable->IsPrimitive()) return true;
    }
    return false;
}


void      VMClass::LoadPrimitives(const vector<pString>& cp,int cp_count)
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
        pString loadstring = genCoreLoadstring(*i);
        dlhandle = loadLib(loadstring);
        //SEND(loadstring, free);
        if(dlhandle != NULL && dlhandle->good() && isResponsible(dlhandle, cname))
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
        loadstring = genLoadstring(*i, cname);
        dlhandle = loadLib(loadstring);
    //    SEND(loadstring, free);
        if(dlhandle != NULL && dlhandle->good()) {
    //        //
    //        // the class library was found...
    //        //
            if(isResponsible(dlhandle, cname)) {
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

//
//void VMClass::MarkReferences()
//{
//	VMObject::MarkReferences();
//	super_class->MarkReferences();
//	name->MarkReferences();
//	instance_fields->MarkReferences();
//	instance_invokables->MarkReferences();
//}

//private Methods

int VMClass::numberOfSuperInstanceFields()
{
	if (this->HasSuperClass()) return this->super_class->GetNumberOfInstanceFields();
	return 0;
}

//LoadPrimitives helper
#define shared_extension ".csp"
pString VMClass::genLoadstring(const pString& cp, 
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
pString VMClass::genCoreLoadstring(const pString& cp) {
    #define S_CORE "SOMCore"
    pString corename = string(S_CORE);
    pString result = genLoadstring(cp, corename);
    //SEND(corename, free);
    
    return result;
}


/**
 * load the given library, return the handle
 *
 */
ifstream* VMClass::loadLib(const pString& path) {
#ifdef __DEBUG
    cout << "loadLib " << path << endl;
#endif
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
bool VMClass::isResponsible(ifstream* handle, const pString& cl) {

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
    for(int i = 0; i < cl->GetNumberOfInstanceInvokables(); i++) {
        
        an_invokable = (VMInvokable*)cl->GetInstanceInvokable(i);
#ifdef __DEBUG
        cout << "cname: >" << cname << "<"<< endl;
        cout << an_invokable->GetSignature()->GetStdString() << endl;
#endif
        if(an_invokable->IsPrimitive()) {
            the_primitive = (VMPrimitive*) an_invokable;
            //
            // we have a primitive to load
            // get it's selector
            //
            VMSymbol* sig =  the_primitive->GetSignature();

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

