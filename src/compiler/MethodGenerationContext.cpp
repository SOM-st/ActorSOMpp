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
}

VMMethod* MethodGenerationContext::Assemble()
{
    // create a method instance with the given number of bytecodes and literals
    int num_literals = this->literals.Size();
    
    VMMethod* meth = _UNIVERSE->NewMethod(this->signature, bytecode.size(), num_literals);
    
    // populate the fields that are immediately available
    int num_locals = this->locals.Size();
    meth->SetNumberOfLocals(num_locals);

    meth->SetMaximumNumberOfStackElements(this->ComputeStackDepth());
#ifdef __DEBUG
    cout << "num_locals: " << num_locals << endl;
    cout << "num_literals: " << num_literals << endl;
#endif
    // copy literals into the method
    for(int i = 0; i < num_literals; i++) {
        VMObject* l = literals.get(i);
        meth->SetIndexableField(i, l);
    }
#ifdef __DEBUG
    cout << "bp: " << bytecode.size();
    cout << "bcs ";
#endif
    // copy bytecodes into method
    for(size_t i = 0; i < bytecode.size(); i++){
        meth->SetBytecode(i, bytecode[i]);
#ifdef __DEBUG
        cout << (int)bytecode[i] << " ";
#endif
    }
#ifdef __DEBUG
    cout << endl;
#endif
    // return the method - the holder field is to be set later on!
    return meth;
}

VMPrimitive* MethodGenerationContext::AssemblePrimitive()
{
    return VMPrimitive::GetEmptyPrimitive(this->signature);
}

MethodGenerationContext::~MethodGenerationContext() {
}

int8_t MethodGenerationContext::FindLiteralIndex(VMObject* lit) {//pVMObject lit) {
	return (int8_t)literals.IndexOf(lit);//literals.IndexOf(lit);

}

bool MethodGenerationContext::FindVar(const pString& var, int* index, int* context, bool* is_argument) {
	if((*index = locals.IndexOf( var)) == -1) {//SEND(mgenc->locals, IndexOfString, var)) == -1) {
        if((*index = arguments.IndexOf( var)) == -1) {
            if(!outer_genc)
                return false;
            else {
                (*context)++;
				return outer_genc->FindVar(var, index,
                    context, is_argument);
            }
        } else
            *is_argument = true;
    }
    
    return true;
}

bool MethodGenerationContext::FindField(const pString& field) {
	return holder_genc->FindField(field);
}
int MethodGenerationContext::GetNumberOfArguments() { return arguments.Size(); };
uint8_t MethodGenerationContext::ComputeStackDepth() {
	uint8_t depth = 0;
    uint8_t max_depth = 0;
    unsigned int i = 0;
    
    while(i < bytecode.size()) {
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
                _UNIVERSE->Quit(1);
        }
        
        if(depth > max_depth)
            max_depth = depth;
    }
    
    return max_depth;
}


void MethodGenerationContext::SetHolder(ClassGenerationContext* holder) {
	holder_genc = holder;
}

void MethodGenerationContext::SetOuter(MethodGenerationContext* outer) {
	outer_genc = outer;
}

void MethodGenerationContext::SetIsBlockMethod(bool is_block) {
	block_method = is_block;
}

void MethodGenerationContext::SetSignature(VMSymbol* sig) {
	signature = sig;
}

void MethodGenerationContext::SetPrimitive(bool prim) {
	primitive = prim;
}

void MethodGenerationContext::AddArgument(const pString& arg) {
	arguments.push_back(arg);
}

void MethodGenerationContext::AddLocal(const pString& local) {
	locals.push_back(local);
}

void MethodGenerationContext::AddLiteral(VMObject* lit) {
	literals.push_back(lit);
}

bool MethodGenerationContext::AddArgumentIfAbsent(const pString& arg) {
	if (locals.IndexOf( arg) != -1) return false;
	arguments.push_back(arg);
	return true;
}

bool MethodGenerationContext::AddLocalIfAbsent(const pString& local) {
	if (locals.IndexOf( local) != -1) return false;
	locals.push_back(local);
	return true;
}

bool MethodGenerationContext::AddLiteralIfAbsent(VMObject* lit) {
	if (literals.IndexOf( lit) != -1) return false;
	literals.push_back(lit);
	return true;
}
void MethodGenerationContext::SetFinished(bool finished) {
	this->finished = finished;
}

ClassGenerationContext* MethodGenerationContext::GetHolder() {
	return holder_genc;
}

MethodGenerationContext* MethodGenerationContext::get_outer() {
	return outer_genc;
}

VMSymbol* MethodGenerationContext::GetSignature() {
	return signature;
}

bool MethodGenerationContext::IsPrimitive() {
	return primitive;
}

bool MethodGenerationContext::IsBlockMethod() {
	return block_method;
}

bool MethodGenerationContext::IsFinished() {
	return finished;
}

void MethodGenerationContext::AddBytecode(uint8_t bc) {
	bytecode.push_back(bc);
}
