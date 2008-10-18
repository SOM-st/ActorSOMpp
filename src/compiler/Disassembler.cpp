
#include <stdio.h>
#include <string.h>

#include "Disassembler.h"

#include "../vm/Universe.h"

#include "../interpreter/bytecodes.h"
#include "../interpreter/Interpreter.h"

#include "../vmobjects/VMArray.h"
#include "../vmobjects/VMBigInteger.h"
#include "../vmobjects/VMBlock.h"
#include "../vmobjects/VMClass.h"
#include "../vmobjects/VMDouble.h"
#include "../vmobjects/VMEvaluationPrimitive.h"
#include "../vmobjects/VMFrame.h"
#include "../vmobjects/VMInteger.h"
#include "../vmobjects/VMInvokable.h"
#include "../vmobjects/VMMethod.h"
#include "../vmobjects/VMObject.h"
#include "../vmobjects/VMPrimitive.h"
#include "../vmobjects/VMString.h"
#include "../vmobjects/VMSymbol.h"
#include "../vmobjects/Signature.h"
#include "../misc/debug.h"
#include "../misc/defs.h"
/** 
 * Dispatch an object to its content and write out
 */
void Disassembler::dispatch(VMObject* o) {
    //dispatch
    // can't switch() objects, so:
    if(!o) return; // NULL isn't interesting.
    else if(o == nil_object)
        debug_print("{Nil}");
    else if(o == true_object)
        debug_print("{True}");
    else if(o == false_object)
        debug_print("{False}"); 
    else if((VMClass*)o == system_class)
        debug_print("{System Class object}");
    else if((VMClass*)o == block_class)
        debug_print("{Block Class object}");
    else if(o == _UNIVERSE->GetGlobal(_UNIVERSE->SymbolForChars("system")))
        debug_print("{System}");
    else {
        VMClass* c = o->GetClass();
        if(c == string_class) {
            //pString s = ((VMString*)o)->GetStdString();
            debug_print("\"%s\"", ((VMString*)o)->GetChars());
        } else if(c == double_class)
            debug_print("%g", ((VMDouble*)o)->GetEmbeddedDouble());
        else if(c == biginteger_class)
            debug_print("%lld", ((VMBigInteger*)o)->GetEmbeddedInteger());
        else if(c == integer_class)
            debug_print("%d", ((VMInteger*)o)->GetEmbeddedInteger());
        else if(c == symbol_class) {
            debug_print("#%s", ((VMSymbol*)o)->GetChars());
        } else
            debug_print("address: %p", (void*)o);
    }
}

/**
 * Dump a class and all subsequent methods.
 */
void Disassembler::Dump(VMClass* cl) {
    for(int i = 0; i < cl->GetNumberOfInstanceInvokables(); ++i) {
        VMInvokable* inv = (VMInvokable*)cl->GetInstanceInvokable(i);
        // output header and skip if the Invokable is a Primitive
        VMSymbol* sig = inv->GetSignature();
        //pString sig_s = SEND(sig, get_string);
        VMSymbol* cname = cl->GetName();
        //pString cname_s = SEND(cname, get_string);
        debug_dump("%s>>%s = ", cname->GetChars(), sig->GetChars());
        if(inv->IsPrimitive()) {
            debug_print("<primitive>\n");
            continue;
        }
        // output actual method
        DumpMethod((VMMethod*)inv, "\t");
    }
}


/**
 * Bytecode Index Accessor macros
 */
#define BC_0 method->GetBytecode(bc_idx)
#define BC_1 method->GetBytecode(bc_idx+1)
#define BC_2 method->GetBytecode(bc_idx+2)


/**
 * Dump all Bytecode of a method.
 */
void Disassembler::DumpMethod(VMMethod* method, const char* indent) {
    debug_print("(\n");
    {   // output stack information
        int locals = method->GetNumberOfLocals(); 
        int max_stack = method->GetMaximumNumberOfStackElements();  
        debug_dump("%s<%d locals, %d stack, %d bc_count>\n", indent, locals, max_stack, method->GetNumberOfBytecodes());
    }
#ifdef _DEBUG
    cout << "bytecodes: ";
      for (int i = 0; i < method->GetNumberOfBytecodes(); ++i)
    {
        cout  << (int)method->GetBytecode(i)<< " ";
    }
    cout << endl;
#endif
    // output bytecodes
    for(int bc_idx = 0; 
        bc_idx < method->GetNumberOfBytecodes(); 
        bc_idx += Bytecode::GetBytecodeLength(method->GetBytecode(bc_idx))
    ) {
        // the bytecode.
        uint8_t bytecode = BC_0;
        // indent, bytecode index, bytecode mnemonic
        debug_dump("%s%4d:%s  ", indent, bc_idx,
            Bytecode::GetBytecodeName(bytecode));
        // parameters (if any)
        if(Bytecode::GetBytecodeLength(bytecode) == 1) {
            debug_print("\n");
            continue;
        }
        switch(bytecode) {
            case BC_PUSH_LOCAL:
                debug_print("local: %d, context: %d\n", BC_1, BC_2); break;
            case BC_PUSH_ARGUMENT:
                debug_print("argument: %d, context %d\n", BC_1, BC_2); break;
            case BC_PUSH_FIELD:{
                VMObject* cst = method->GetConstant(bc_idx);
                
                //pString s_name = SEND(name, get_string);
                if (cst != NULL) {
                    VMSymbol* name = dynamic_cast<VMSymbol*>(cst);
                    if (name != NULL) {
                        debug_print("(index: %d) field: %s\n", BC_1, name->GetChars());
                        break;
                    }
                } else
                    debug_print("(index: %d) field: !NULL!: error!\n", BC_1);
                break;
            }
            case BC_PUSH_BLOCK: {
                char* nindent = new char[strlen(indent)+1+1];
                debug_print("block: (index: %d) ", BC_1);
                sprintf(nindent, "%s\t", indent);
                
                Disassembler::DumpMethod(
                    (VMMethod*)(method->GetConstant(bc_idx)), nindent);
                break;
            }            
            case BC_PUSH_CONSTANT: {
                VMObject* constant = method->GetConstant(bc_idx);
                VMClass* cl = constant->GetClass();
                VMSymbol* cname = cl->GetName();
                //pString s_cname = SEND(cname, get_string);
                debug_print("(index: %d) value: (%s) ", 
                            BC_1, cname->GetChars());
                dispatch(constant); debug_print("\n");
                break;
            }
            case BC_PUSH_GLOBAL: {
                VMObject* cst = method->GetConstant(bc_idx);
                
                //pString s_name = SEND(name, get_string);
                if (cst != NULL) {
                    VMSymbol* name = dynamic_cast<VMSymbol*>(cst);
                    if (name != NULL) {
                        debug_print("(index: %d) value: %s\n", BC_1, name->GetChars());
                        break;
                    }
                } else
                    debug_print("(index: %d) value: !NULL!: error!\n", BC_1);
                
                break;
            }
            case BC_POP_LOCAL:
                debug_print("local: %d, context: %d\n", BC_1, BC_2);
                break;
            case BC_POP_ARGUMENT:
                debug_print("argument: %d, context: %d\n", BC_1, BC_2);
                break;
            case BC_POP_FIELD: {
                VMSymbol* name = (VMSymbol*)(method->GetConstant(bc_idx));
                //pString s_name = SEND(name, get_string);
                debug_print("(index: %d) field: %s\n", BC_1, name->GetChars());
                break;
            }
            case BC_SEND: {
                VMSymbol* name = (VMSymbol*)(method->GetConstant(bc_idx));
                //pString s_name = SEND(name, get_string);
                debug_print("(index: %d) signature: %s\n", BC_1,
                    name->GetChars());
                break;
            }
            case BC_SUPER_SEND: {
                VMSymbol* name = (VMSymbol*)(method->GetConstant(bc_idx));
                //pString s_name = SEND(name, get_string);
                debug_print("(index: %d) signature: %s\n", BC_1,
                    name->GetChars());
                break;
            }
            default:
                debug_print("<incorrect bytecode>\n");
        }
    }
    debug_dump("%s)\n", indent);
}

/**
 * Dump bytecode from the frame running
 */
void Disassembler::DumpBytecode(VMFrame* frame, VMMethod* method, int bc_idx) {
    static long long indentc = 0;
    static char      ikind   = '@';
    uint8_t          bc      = BC_0;
    VMObject*         clo   = method->GetHolder();
    VMClass* cl = dynamic_cast<VMClass*>(clo);
    // Determine Context: Class or Block?
    if(cl != NULL) {
        VMSymbol* cname = cl->GetName();
        VMSymbol* sig = method->GetSignature();
        //pString s_cname = SEND(cname, get_string);
        //pString s_sig = SEND(sig, get_string);
        
        debug_trace("%20s>>%-20s% 10lld %c %04d: %s\t",
                    cname->GetChars(), sig->GetChars(),
                    indentc, ikind, bc_idx,
                    Bytecode::GetBytecodeName(bc));        
    } else {
        VMSymbol* sig = method->GetSignature();
        //pString s_sig = SEND(sig, get_string);
        debug_trace("%-42s% 10lld %c %04d: %s\t", 
                    sig->GetChars(),
                    indentc, ikind, bc_idx,
                    Bytecode::GetBytecodeName(bc));
    }
    // reset send indicator
    if(ikind != '@') ikind = '@';
    
    switch(bc) {
        case BC_HALT: {
            debug_print("<halting>\n\n\n");
            break;
        }
        case BC_DUP: {
            VMObject* o = frame->GetStackElement(0);
            if(o) {
                VMClass* c = o->GetClass();
                VMSymbol* cname = c->GetName();
                //pString s_cname = SEND(cname, get_string);
                debug_print("<to dup: (%s) ", cname->GetChars());
                //dispatch
                dispatch(o);
            } else
                debug_print("<to dup: address: %p", (void*)o);
            debug_print(">\n");                        
            break;
        }
        case BC_PUSH_LOCAL: {
            uint8_t bc1 = BC_1, bc2 = BC_2;
            VMObject* o = frame->GetLocal(bc1, bc2);
            VMClass* c = o->GetClass();
            VMSymbol* cname = c->GetName();
            //pString s_cname = SEND(cname, get_string);
            debug_print("local: %d, context: %d <(%s) ", 
                        BC_1, BC_2, cname->GetChars());
            //dispatch
            dispatch(o);
            debug_print(">\n");                        
            break;
        }
        case BC_PUSH_ARGUMENT: {
            uint8_t bc1 = BC_1, bc2 = BC_2;
            VMObject* o = frame->GetArgument(bc1, bc2);
            debug_print("argument: %d, context: %d", bc1, bc2);
            if(dynamic_cast<VMClass*>(cl) != NULL) {
                VMClass* c = o->GetClass();
                VMSymbol* cname = c->GetName();
                //pString s_cname = SEND(cname, get_string);
                debug_print("<(%s) ", cname->GetChars());
                //dispatch
                dispatch(o);                
                debug_print(">");                        
            }            
            debug_print("\n");
            break;
        }
        case BC_PUSH_FIELD: {
            VMFrame* ctxt = frame->GetOuterContext();
            VMObject* arg = ctxt->GetArgument(0, 0);
            VMSymbol* name = (VMSymbol*)(method->GetConstant(bc_idx));
            int field_index = arg->GetFieldIndex(name);
           
            VMObject* o = arg->GetField(field_index);
            VMClass* c = o->GetClass();
            VMSymbol* cname = c->GetName();
            //pString s_name = SEND(name, get_string);
            //pString s_cname = SEND(cname, get_string);

            debug_print("(index: %d) field: %s <(%s) ", BC_1,
                        name->GetChars(), cname->GetChars());
            //dispatch
            dispatch(o);                
            debug_print(">\n");                        
            break;
        }
        case BC_PUSH_BLOCK: {
            debug_print("block: (index: %d) ", BC_1);
            VMMethod* meth = (VMMethod*)(method->GetConstant(bc_idx));
            DumpMethod(meth, "$");
            break;
        }
        case BC_PUSH_CONSTANT: {
            VMObject* constant = method->GetConstant(bc_idx);
            VMClass* c = constant->GetClass();
            VMSymbol* cname = c->GetName();
            //pString s_cname = SEND(cname, get_string);
            debug_print("(index: %d) value: (%s) ", BC_1, 
                        cname->GetChars());
            dispatch(constant);
            debug_print("\n");
            break;
        }
        case BC_PUSH_GLOBAL: {
            VMSymbol* name = (VMSymbol*)(method->GetConstant(bc_idx));
            VMObject* o = _UNIVERSE->GetGlobal(name);
            VMSymbol* cname;
            //pString s_cname;
            char*   c_cname;
            if(o) {
                VMClass* c = o->GetClass();
                cname = c->GetName();
                //s_cname = SEND(cname, get_string);
                c_cname = cname->GetChars();
            } else
                c_cname = "NULL";
            
            //pString s_name =SEND(name, get_string);
            debug_print("(index: %d)value: %s <(%s) ", BC_1,
                        name->GetChars(), c_cname);
            dispatch(o);
            debug_print(">\n");
            break;
        }
        case BC_POP: {
            size_t sp = frame->GetStackPointer()->GetEmbeddedInteger();
            VMObject* o = ((VMArray*)frame)->GetIndexableField(sp);
            VMClass* c = o->GetClass();
            VMSymbol* cname = c->GetName();
            //pString s_cname = SEND(cname, get_string);
            debug_print("popped <(%s) ", cname->GetChars());
            //dispatch
            dispatch(o);
            debug_print(">\n");                        
            break;            
        }            
        case BC_POP_LOCAL: {
            size_t sp = frame->GetStackPointer()->GetEmbeddedInteger();
            VMObject* o = ((VMArray*)frame)->GetIndexableField(sp);
            VMClass* c = o->GetClass();
            VMSymbol* cname = c->GetName();
            //pString s_cname = SEND(cname, get_string);
            debug_print("popped local: %d, context: %d <(%s) ", BC_1, BC_2,
                        cname->GetChars());
            //dispatch
            dispatch(o);            
            debug_print(">\n");                        
            break;            
        }
        case BC_POP_ARGUMENT: {
            size_t sp = frame->GetStackPointer()->GetEmbeddedInteger();
            VMObject* o = ((VMArray*)frame)->GetIndexableField(sp);
            VMClass* c = o->GetClass();
            VMSymbol* cname = c->GetName();
            debug_print("argument: %d, context: %d <(%s) ", BC_1, BC_2,
                        cname->GetChars());
            //dispatch
            dispatch(o);
            debug_print(">\n");                        
            break;
        }
        case BC_POP_FIELD: {
            size_t sp = frame->GetStackPointer()->GetEmbeddedInteger();
            VMObject* o = ((VMArray*)frame)->GetIndexableField(sp);
            VMSymbol* name = (VMSymbol*)(method->GetConstant(bc_idx));
            VMClass* c = o->GetClass();
            VMSymbol* cname = c->GetName();
            //pString s_name = SEND(name, get_string);
            //pString s_cname = SEND(cname, get_string);
            debug_print("(index: %d) field: %s <(%s) ",  BC_1,
                        name->GetChars(),
                        cname->GetChars());
            dispatch(o);
            debug_print(">\n");                        
            break;
        }
        case BC_SUPER_SEND:
        case BC_SEND: {
            VMSymbol* sel = (VMSymbol*)(method->GetConstant(bc_idx));
            //pString s_sel = SEND(sel, get_string);

            debug_print("(index: %d) signature: %s (", BC_1,
                        sel->GetChars());
            //handle primitives, they don't increase call-depth
            VMObject* elem = _UNIVERSE->GetInterpreter()->GetFrame()->GetStackElement(
                                                Signature::GetNumberOfArguments(sel)-1);
            VMClass* elemClass = elem->GetClass();
            VMInvokable* inv =  dynamic_cast<VMInvokable*>(elemClass->LookupInvokable(sel));
            
            if(inv != NULL && inv->IsPrimitive()) 
                debug_print("*)\n");
            else {
                debug_print("\n");    
                indentc++; ikind='>'; // visual
            }
                break;
        }            
        case BC_RETURN_LOCAL:
        case BC_RETURN_NON_LOCAL: {
            debug_print(")\n");
            indentc--; ikind='<'; //visual
            break;
        }
        default:
            debug_print("<incorrect bytecode>\n");
            break;
    }
}

// EOF: diassembler.c

