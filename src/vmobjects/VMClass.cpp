#include "VMClass.h"
#include "VMArray.h"
#include "VMSymbol.h"
#include "VMInvokable.h"

bool VMClass::add_instance_invokable(VMObject *ptr)
{
	for (int i = 0; i < instance_invokables->GetNumberOfIndexableFields(); ++i)
	{
		VMObject* cmp = instance_invokables->GetItem(i);
		if (cmp != 0) {
			//if (ptr->GetSignature() == ((VMInvokable*)cmp)->GetSignature()) {
			//	  instance_invokables->AddItem(i, ptr);
			//	  return false;
			//}
		}
	}
	if (instance_invokables->GetNumberOfIndexableFields() >= instance_invokables->GetArraySize())
		instance_invokables = instance_invokables->CopyAndExtendWith(ptr);
	else instance_invokables->AddItem(ptr);

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
	instance_invokables->AddItem(index, invokable);
	//instance_invokables[index] = invokable;
}

VMObject* VMClass::lookup_invokable(VMSymbol* name)
{
    VMInvokable* invokable = NULL;
    for (int i = 0; i < get_number_of_instance_invokables(); ++i)
    {
        invokable = (VMInvokable*)(get_instance_invokable(i));
        if (invokable->get_signature() == name) return (VMObject*)invokable;
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
    for (int i = get_number_of_instance_fields() - 1; i >= 0; --i)
    {
        if (name == this->get_instance_field_name(i)) return i;
    }
	return -1;
}


int       VMClass::get_number_of_instance_fields()
{
	return instance_fields->GetNumberOfIndexableFields();
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

void      VMClass::load_primitives(const pString* name,int cp_count)
{
    //// the library handle
    //void* dlhandle=NULL;
    //
    //// cached object properties
    //pString cname = this->name->GetStdString;

    //// iterate the classpathes
    //for(int i = 0; (i < cp_count) && !dlhandle; i++) {
    //    //
    //    // check the core library
    //    //
    //    
    //    
    //    pString loadstring = gen_core_loadstring(cp[i]);
    //    dlhandle = load_lib(loadstring);
    //    SEND(loadstring, free);
    //    if(dlhandle && is_responsible(dlhandle, cname))
    //        //
    //        // the core library is found and responsible
    //        //
    //        break;
    //    
    //    
    //    //
    //    // the core library is not found or respondible, 
    //    // continue w/ class file
    //    //
    //    
    //    
    //    loadstring = gen_loadstring(cp[i], cname);
    //    dlhandle = load_lib(loadstring);
    //    SEND(loadstring, free);
    //    if(dlhandle) {
    //        //
    //        // the class library was found...
    //        //
    //        if(is_responsible(dlhandle, cname)) {
    //            //
    //            // ...and is responsible.
    //            //
    //            break;
    //        } else {
    //            //
    //            // ... but says not responsible, but have to
    //            // close it
    //            //
    //            dlclose(dlhandle);
    //            Universe_error_exit("Library claims no responsiblity, "
    //                                "but musn't!");        
    //        }
    //    }        
    //    /*
    //     * continue checking the next class path
    //     *
    //     */
    //}

    ////
    //// finished cycling,
    //// check if a lib was found.
    ////
    //if(!dlhandle) {
    //    debug_error("load failure: %s\n", dlerror());
    //    debug_error("could not load primitive library for %s\n",
    //                SEND(cname, chars));
    //    Universe_exit(ERR_FAIL);
    //}
    //
    //
    ///*
    // * do the actual loading for both class and metaclass
    // *
    // */
    //set_primitives(self, dlhandle, cname, INSTANCE_METHOD_FORMAT_S);
    //set_primitives(self->class, dlhandle, cname, CLASS_METHOD_FORMAT_S);
}


void VMClass::MarkReferences()
{
	VMObject::MarkReferences();
	super_class->MarkReferences();
	name->MarkReferences();
	instance_fields->MarkReferences();
	instance_invokables->MarkReferences();
}

int VMClass::numberOfSuperInstanceFields()
{
	if (this->has_super_class()) return this->super_class->get_number_of_instance_fields();
	return 0;
}