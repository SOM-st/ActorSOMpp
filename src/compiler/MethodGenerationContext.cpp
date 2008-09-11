#include "MethodGenerationContext.h"
#include "../interpreter/bytecodes.h"
#include "../vmobjects/VMSymbol.h"
#include "../vmobjects/VMMethod.h"
#include "../vmobjects/Signature.h"
#include "../vmobjects/VMMethod.h"
#include "../vmobjects/VMPrimitive.h"

MethodGenerationContext::MethodGenerationContext() {
	//signature = 0;
	holder_genc = 0;
	outer_genc = 0;
	this->arguments.Clear();
	this->literals.Clear();
	this->locals.Clear();
    this->bytecode.clear();
	primitive = false;
	block_method = false;
	finished = false;
	bp = 0;
}

VMMethod* MethodGenerationContext::Assemble()
{
    // create a method instance with the given number of bytecodes and literals
    int num_literals = this->literals.Size();
    VMMethod* meth = _UNIVERSE->new_method(this->signature, bp, num_literals);
    
    // populate the fields that are immediately available
    int num_locals = this->locals.Size();
    meth->set_number_of_locals(num_locals);

    meth->set_maximum_number_of_stack_elements(this->compute_stack_depth());
    cout << "num_locals: " << num_locals << endl;
    cout << "num_literals: " << num_literals << endl;
    // copy literals into the method
    for(int i = 0; i < num_literals; i++) {
        VMObject* l = literals.get(i);
        meth->SetLiteral(i, l);
    }
    
    cout << "bp: " << bp;
    cout << "bcs ";
    // copy bytecodes into method
    for(size_t i = 0; i < bp; i++){
        meth->set_bytecode(i, bytecode[i]);
        cout << bytecode[i] << " ";}
    cout << endl;
    // return the method - the holder field is to be set later on!
    return meth;
}

VMPrimitive* MethodGenerationContext::AssemblePrimitive()
{
    return VMPrimitive::GetEmptyPrimitive(this->signature);
}

MethodGenerationContext::~MethodGenerationContext() {
}

int8_t MethodGenerationContext::find_literal_index(VMObject* lit) {//pVMObject lit) {
	return (int8_t)literals.IndexOf(lit);//literals.IndexOf(lit);

}

bool MethodGenerationContext::find_var(const pString& var, int* index, int* context, bool* is_argument) {
	if((*index = locals.IndexOf( var)) == -1) {//SEND(mgenc->locals, IndexOfString, var)) == -1) {
        if((*index = locals.IndexOf( var)) == -1) {
            if(!outer_genc)
                return false;
            else {
                (*context)++;
				return outer_genc->find_var(var, index,
                    context, is_argument);
            }
        } else
            *is_argument = true;
    }
    
    return true;
}

bool MethodGenerationContext::find_field(const pString& field) {
	return holder_genc->find_field(field);
}
int MethodGenerationContext::get_number_of_arguments() { return arguments.Size(); };
uint8_t MethodGenerationContext::compute_stack_depth() {
	uint8_t depth = 0;
    uint8_t max_depth = 0;
    unsigned int i = 0;
    
    while(i < bp) {
        switch(bytecode[i]) {
            case BC_HALT             :          i++;    break;
            case BC_DUP              : depth++; i++;    break;
            case BC_PUSH_LOCAL       :
            case BC_PUSH_ARGUMENT    : depth++; i += 3; break;
            case BC_PUSH_FIELD       :
            case BC_PUSH_BLOCK       :
            case BC_PUSH_CONSTANT    :
            case BC_PUSH_GLOBAL      : depth++; i += 2; break;
            case BC_POP              : depth--; i++;    break;
            case BC_POP_LOCAL        :
            case BC_POP_ARGUMENT     : depth--; i += 3; break;
            case BC_POP_FIELD        : depth--; i += 2; break;
            case BC_SEND             :
            case BC_SUPER_SEND       : {
                // these are special: they need to look at the number of
                // arguments (extractable from the signature)
                VMSymbol* sig = (VMSymbol*)literals.get(bytecode[i + 1]);
                    //SEND(mgenc->literals, get, mgenc->bytecode[i + 1]);
                
                depth -= Signature::GetNumberOfArguments(sig);
                
				depth++; // return value
                i += 2;
                break;
            }
            case BC_RETURN_LOCAL     :
            case BC_RETURN_NON_LOCAL :          i++;    break;
            default                  :
               // debug_error("Illegal bytecode %d.\n", bytecode[i]);
                //Universe_exit(1);
                cout << "Illegal bytecode: " << bytecode[i];
                _UNIVERSE->quit(1);
        }
        
        if(depth > max_depth)
            max_depth = depth;
    }
    
    return max_depth;
}


void MethodGenerationContext::set_holder(class_generation_context* holder) {
	holder_genc = holder;
}

void MethodGenerationContext::set_outer(MethodGenerationContext* outer) {
	outer_genc = outer;
}

void MethodGenerationContext::set_is_block_method(bool is_block) {
	block_method = is_block;
}

void MethodGenerationContext::set_signature(VMSymbol* sig) {
	signature = sig;
}

void MethodGenerationContext::set_primitive(bool prim) {
	primitive = prim;
}

void MethodGenerationContext::add_argument(const pString& arg) {
	arguments.push_back(arg);
}

void MethodGenerationContext::add_local(const pString& local) {
	locals.push_back(local);
}

void MethodGenerationContext::add_literal(VMObject* lit) {
	literals.push_back(lit);
}

bool MethodGenerationContext::add_argument_if_absent(const pString& arg) {
	if (locals.IndexOf( arg) != -1) return false;
	arguments.push_back(arg);
	return true;
}

bool MethodGenerationContext::add_local_if_absent(const pString& local) {
	if (locals.IndexOf( local) != -1) return false;
	locals.push_back(local);
	return true;
}

bool MethodGenerationContext::add_literal_if_absent(VMObject* lit) {
	if (literals.IndexOf( lit) != -1) return false;
	literals.push_back(lit);
	return true;
}
void MethodGenerationContext::set_finished(bool finished) {
	this->finished = finished;
}

class_generation_context* MethodGenerationContext::get_holder() {
	return holder_genc;
}

MethodGenerationContext* MethodGenerationContext::get_outer() {
	return outer_genc;
}

VMSymbol* MethodGenerationContext::get_signature() {
	return signature;
}

bool MethodGenerationContext::is_primitive() {
	return primitive;
}

bool MethodGenerationContext::is_block_method() {
	return block_method;
}

bool MethodGenerationContext::is_finished() {
	return finished;
}

void MethodGenerationContext::add_bytecode(uint8_t bc) {
	bytecode.push_back(bc);
    ++bp;
}