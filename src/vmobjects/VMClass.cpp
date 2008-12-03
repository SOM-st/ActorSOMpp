#include "VMClass.h"
#include "VMArray.h"
#include "VMSymbol.h"
#include "VMInvokable.h"
#include "VMPrimitive.h"
#include "PrimitiveRoutine.h"
#include <fstream>
#include <typeinfo>


#if defined(__GNUC__)
#   include <dlfcn.h>
#else   //Visual Studio
/**
 * Emualting the dl-interface with win32 means
 */
#   define WIN32_LEAN_AND_MEAN
#   define dlerror()   "Load Error"
#   define dlsym       GetProcAddress
#   define DL_LOADMODE NULL, LOAD_WITH_ALTERED_SEARCH_PATH
#   define dlopen      LoadLibrary
#   define dlclose     FreeLibrary
//#   include <windows.h> //included in VMClass.h if necessary
#endif


/*
 * Format definitions for Primitive naming scheme.
 *
 */
#define CLASS_METHOD_FORMAT_S "%s::%s"
// as in AClass::aClassMethod
#define INSTANCE_METHOD_FORMAT_S "%s::%s_"
// as in AClass::anInstanceMethod_


VMClass::VMClass() : VMObject(4) {
    //this->objectSize = sizeof(VMClass);
}


VMClass::VMClass( int number_of_fields ) : VMObject(number_of_fields + 4) {
    //this->objectSize = sizeof(VMClass) + number_of_fields*sizeof(VMObject*);
}


bool VMClass::HasSuperClass() const {
    return (super_class != NULL && super_class != Globals::NilObject());
}


bool VMClass::AddInstanceInvokable(VMObject *ptr) {
    VMInvokable* newInvokable = dynamic_cast<VMInvokable*>(ptr);
    if (newInvokable == NULL) {
        cout << "Error: trying to add non-invokable to invokables array" << endl;
        throw std::bad_typeid();//("Trying to add non-invokable to invokables array");
    }
	for (int i = 0; i < instance_invokables->GetNumberOfIndexableFields(); ++i) {
        VMInvokable* inv = dynamic_cast<VMInvokable*>( (*instance_invokables)[i] );
		if (inv != 0) {
            if (newInvokable->GetSignature() == inv->GetSignature()) {
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


void VMClass::AddInstancePrimitive(VMPrimitive *ptr) {
	if (AddInstanceInvokable((VMObject*)ptr)) {
		//cout << "Warn: Primitive "<<ptr->GetSignature<<" is not in class definition for class " << name->GetStdString() << endl;
	}
}


VMSymbol* VMClass::GetInstanceFieldName(int index) const {
	if (index >= numberOfSuperInstanceFields()) {
		index -= numberOfSuperInstanceFields();
		return (VMSymbol*) (*instance_fields)[index];
	}
	
	return super_class->GetInstanceFieldName(index);
}




void      VMClass::SetInstanceInvokables(VMArray* invokables) {

	instance_invokables = invokables;

    for (int i = 0; i < this->GetNumberOfInstanceInvokables(); ++i) {
        VMInvokable* inv = (VMInvokable*)(*instance_invokables)[i];
        if (inv != Globals::NilObject()) {
            inv->SetHolder(this);
        }
    }

}


int       VMClass::GetNumberOfInstanceInvokables() const {
	return instance_invokables->GetNumberOfIndexableFields();
}


VMObject *VMClass::GetInstanceInvokable(int index) const {
    return (*instance_invokables)[index];
}


void      VMClass::SetInstanceInvokable(int index, VMObject* invokable) {
	instance_invokables->SetIndexableField(index, invokable);
    if (invokable != Globals::NilObject()) {
        VMInvokable* inv = (VMInvokable*) invokable;
        inv->SetHolder(this);
    }
	//instance_invokables[index] = invokable;
}


VMObject* VMClass::LookupInvokable(VMSymbol* name) const {
    VMInvokable* invokable = NULL;
    for (int i = 0; i < GetNumberOfInstanceInvokables(); ++i) {
        invokable = (VMInvokable*)(GetInstanceInvokable(i));
        if (invokable->GetSignature() == name) 
            return (VMObject*)invokable;
    }
    invokable = NULL;
    //look in super class
    if (this->HasSuperClass())  {
        invokable = (VMInvokable*)this->super_class->LookupInvokable(name);
    }
	return (VMObject*)invokable;
}


int       VMClass::LookupFieldIndex(VMSymbol* name) const {
    for (int i = 0; i <=GetNumberOfInstanceFields(); ++i) { 
        //even with GetNumberOfInstanceFields == 0 there is the class field 
        if (name == this->GetInstanceFieldName(i) ||
            name->GetStdString() == this->GetInstanceFieldName(i)->GetStdString()) 
                return i;
    }
	return -1;
}


int       VMClass::GetNumberOfInstanceFields() const {
	return instance_fields->GetNumberOfIndexableFields()
           + this->numberOfSuperInstanceFields();
}


bool      VMClass::HasPrimitives() const {
	for (int i = 0; i < GetNumberOfInstanceInvokables(); ++i) {
        VMInvokable* invokable = (VMInvokable*)(GetInstanceInvokable(i));
        if (invokable->IsPrimitive()) return true;
    }
    return false;
}


void      VMClass::LoadPrimitives(const vector<StdString>& cp,int cp_count) {

    // the library handle
    //ifstream* dlhandle = NULL;
#if defined(__GNUC__)
    void* dlhandle=NULL;
#else
    HMODULE dlhandle = NULL;
#endif
    //
    // cached object properties
    StdString cname = this->name->GetStdString();
    //StdString cname = this->name->GetStdString;

    //// iterate the classpathes
    for(vector<StdString>::const_iterator i = cp.begin(); (i != cp.end()) && dlhandle == NULL; ++i) {
        // check the core library
        StdString loadstring = genCoreLoadstring(*i);
        dlhandle = loadLib(loadstring);
        if(dlhandle != NULL) {

#if defined(_MSC_VER)
            //initialize core (core has to make sure not to initialize twice -> Core.cpp)
            //Where / How is this done in CSOM?????????
            Setup* setup = (Setup*) dlsym(dlhandle, "setup");
            const char* dlsym_error = dlerror();
            if (!setup) {
                cerr << "Cannot load Core library: " << dlsym_error << '\n';
                dlclose(dlhandle);
                _UNIVERSE->ErrorExit("Core library does not define the setup() initializer.");
            }
            //call the setup function to intialize the class library
            VMObject* g[] = {Globals::TrueObject(), Globals::FalseObject(), Globals::NilObject()};
            setup(_UNIVERSE, _HEAP, g);
#endif

            if(isResponsible(dlhandle, cname))
            // the core library is found and responsible
            
            break;
        }
   
        // the core library is not found or not responsible, 
        // continue w/ class file
        loadstring = genLoadstring(*i, cname);
        cout << loadstring.c_str() << endl;
        dlhandle = loadLib(loadstring);
        if(dlhandle != NULL) {
             cout << "2222222222222222" << endl;
            //
            // the class library was found...
            //
            if(isResponsible(dlhandle, cname)) {
                //
                // ...and is responsible.
                //
                //Setup* setup = (Setup*) dlsym(dlhandle, "setup");
                //const char* dlsym_error = dlerror();
                //if (!setup) {
                //    cerr << "Cannot load symbol \"setup\": " << dlsym_error << '\n';
                //    dlclose(dlhandle);
                //    _UNIVERSE->ErrorExit("Library does not define the setup() initializer.");
                //}
                ////call the setup function to intialize the class library
                //setup();
                break;
            } else {
                //
                // ... but says not responsible, but we have to
                // close it nevertheless
                //
                dlclose(dlhandle);
                _UNIVERSE->ErrorExit("Library claims no resonsibility, but musn't!");
            }
            
        }
        /*
         * continue checking the next class path
         */
    }

    // finished cycling,
    // check if a lib was found.
    if(dlhandle == NULL) {
        cout << "load failure: ";
        cout << "could not load primitive library for " << cname << endl;
        _UNIVERSE->Quit(ERR_FAIL);
    }
    
    ///*
    // * do the actual loading for both class and metaclass
    // *
    // */
    set_primitives(dlhandle, cname);
    GetClass()->set_primitives(dlhandle, cname);
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

int VMClass::numberOfSuperInstanceFields() const {
	if (this->HasSuperClass()) 
        return this->super_class->GetNumberOfInstanceFields();
	return 0;
}

//LoadPrimitives helper
#define shared_extension ".csp"

StdString VMClass::genLoadstring(const StdString& cp, 
                       const StdString& cname
                       ) const {
    
    StdString loadstring = string(cp);
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
StdString VMClass::genCoreLoadstring(const StdString& cp) const {
    #define S_CORE "SOMCore"
    StdString corename = string(S_CORE);
    StdString result = genLoadstring(cp, corename);
    //SEND(corename, free);
    
    return result;
}


/**
 * load the given library, return the handle
 *
 */
#if defined(__GNUC__)

void* VMClass::loadLib(const StdString& path) const {

#else

HMODULE VMClass::loadLib(const StdString& path) const {

#endif

#ifdef __DEBUG
    cout << "loadLib " << path << endl;
#endif
    #if defined(__GNUC__)
        #ifdef DEBUG
            #define    DL_LOADMODE RTLD_NOW
        #else
            #define    DL_LOADMODE RTLD_LAZY
        #endif
    #endif
    
    // static handle. will be returned
#if defined(__GNUC__)
    void* dlhandle;
    // try load lib
    if((dlhandle=dlopen(path.c_str(), DL_LOADMODE))) {
#else
    HMODULE dlhandle;
    // try load lib
    const char* filename = path.c_str();
    

    int length = MultiByteToWideChar(CP_ACP,0,filename ,strlen(filename)+1,NULL,0);
    LPWSTR buffer = (LPWSTR)malloc(sizeof(WCHAR)*length);
    MultiByteToWideChar(CP_ACP,0,filename ,strlen(filename)+1,buffer,length);
    if((dlhandle=dlopen(buffer))) {
        free(buffer);
#endif
    
	
		//found.
		return dlhandle;
    } else {
        cout << "Error loading library "<<path<<": " << dlerror() << endl;
        return NULL;
    }
}


/**
 * check, whether the lib referenced by the handle supports the class given
 *
 */
#if defined(__GNUC__)

bool VMClass::isResponsible(void* dlhandle, const StdString& cl) const {

#else

bool VMClass::isResponsible(HMODULE dlhandle, const StdString& cl) const {

#endif

    // function handler
    SupportsClass* supports_class=NULL;

    supports_class = (SupportsClass*)dlsym(dlhandle, "supportsClass");
	if(!supports_class) {
        cout << "error: " << dlerror() << endl;
        /*LPTSTR pszMessage;
        DWORD dwLastError = GetLastError(); 

        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | 
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            dwLastError,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&pszMessage,
            0, NULL );

        wprintf(L"failed with error %d: %s\n", dwLastError, pszMessage);

        LocalFree(pszMessage);*/
    
        _UNIVERSE->ErrorExit("Library doesn't have expected format: ");
    }
    
    // test class responsibility
    return supports_class(cl.c_str());
}




/*
 * set the routines for primitive marked invokables of the given class
 *
 */
#if defined(__GNUC__)
void VMClass::set_primitives(void* dlhandle, const StdString& cname) {    
#else
void VMClass::set_primitives(HMODULE dlhandle, 
                             const StdString& cname) {    
#endif
    VMPrimitive* the_primitive;
    PrimitiveRoutine*   routine=NULL;
    VMInvokable* an_invokable;
    // iterate invokables
    for(int i = 0; i < this->GetNumberOfInstanceInvokables(); i++)  {
        
        an_invokable = (VMInvokable*)this->GetInstanceInvokable(i);
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

            StdString selector = sig->GetPlainString();
            
            CreatePrimitive* create = 
                                (CreatePrimitive*) dlsym(dlhandle, "create");
            routine = create(cname, selector);
            
            if(!routine) {
                cout << "could not load primitive '"<< selector 
                     <<"' for class " << cname << endl;
                            
                _UNIVERSE->Quit(ERR_FAIL);
            }
            
            // set routine
            the_primitive->SetRoutine(routine);
            the_primitive->SetEmpty(false);
        }
    }
}

