#include "MethodGenerationContext.h"
#include "../interpreter/bytecodes.h"

method_generation_context::method_generation_context() {
	//signature = 0;
	holder_genc = 0;
	outer_genc = 0;
	this->arguments.clear();
	this->literals.clear();
	this->locals.clear();
	primitive = false;
	block_method = false;
	finished = false;
	bp = 0;
}

method_generation_context::~method_generation_context() {
}

int8_t method_generation_context::find_literal_index(pVMSymbol lit) {//pVMObject lit) {
	return (int8_t)index_of(literals, lit);//literals.index_of(lit);

}

bool method_generation_context::find_var(pString var, int* index, int* context, bool* is_argument) {
	if((*index = index_of(locals, var)) == -1) {//SEND(mgenc->locals, indexOfString, var)) == -1) {
        if((*index = index_of(arguments, var)) == -1) {
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

bool method_generation_context::find_field(pString field) {
	return holder_genc->find_field(field);
}
int method_generation_context::get_number_of_arguments() { return arguments.size(); };
uint8_t method_generation_context::compute_stack_depth() {
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
                pVMSymbol sig = literals.at(bytecode[i + 1]);
                    //SEND(mgenc->literals, get, mgenc->bytecode[i + 1]);
                
				//depth -= Signature_get_number_of_arguments(sig);
                
				depth++; // return value
                i += 2;
                break;
            }
            case BC_RETURN_LOCAL     :
            case BC_RETURN_NON_LOCAL :          i++;    break;
            default                  :
               // debug_error("Illegal bytecode %d.\n", bytecode[i]);
                //Universe_exit(1);
				exit(1);
        }
        
        if(depth > max_depth)
            max_depth = depth;
    }
    
    return max_depth;
}


void method_generation_context::set_holder(class_generation_context* holder) {
	holder_genc = holder;
}

void method_generation_context::set_outer(method_generation_context* outer) {
	outer_genc = outer;
}

void method_generation_context::set_is_block_method(bool is_block) {
	block_method = is_block;
}

void method_generation_context::set_signature(pVMSymbol sig) {
	signature = sig;
}

void method_generation_context::set_primitive(bool prim) {
	primitive = prim;
}

void method_generation_context::add_argument(pString arg) {
	arguments.push_back(arg);
}

void method_generation_context::add_local(pString local) {
	locals.push_back(local);
}

void method_generation_context::add_literal(pString lit) {
	literals.push_back(lit);
}

bool method_generation_context::add_argument_if_absent(pString arg) {
	if (index_of(arguments, arg) != -1) return false;
	arguments.push_back(arg);
	return true;
}

bool method_generation_context::add_local_if_absent(pString local) {
	if (index_of(locals, local) != -1) return false;
	locals.push_back(local);
	return true;
}

bool method_generation_context::add_literal_if_absent(pString lit) {
	if (index_of(literals, lit) != -1) return false;
	literals.push_back(lit);
	return true;
}
void method_generation_context::set_finished(bool finished) {
	this->finished = finished;
}

class_generation_context* method_generation_context::get_holder() {
	return holder_genc;
}

method_generation_context* method_generation_context::get_outer() {
	return outer_genc;
}

pVMSymbol method_generation_context::get_signature() {
	return signature;
}

bool method_generation_context::is_primitive() {
	return primitive;
}

bool method_generation_context::is_block_method() {
	return block_method;
}

bool method_generation_context::is_finished() {
	return finished;
}

void method_generation_context::add_bytecode(uint8_t bc) {
	bytecode[bp++] = bc;
}