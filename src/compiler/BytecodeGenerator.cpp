#include "BytecodeGenerator.h"



//#include "../vmobjects/VMMethod.h"
//#include "../vmobjects/VMSymbol.h"
//#include "../vmobjects/VMObject.h"
//#include "../vmobjects/VMString.h"

#define EMIT1(BC) \
    mgenc->add_bytecode(BC)
    //mgenc->bytecode[mgenc->bp++] = (BC)

#define EMIT2(BC, IDX) \
    mgenc->add_bytecode(BC);\
	mgenc->add_bytecode(IDX)
    //mgenc->bytecode[mgenc->bp++] = (BC); \
	//mgenc->bytecode[mgenc->bp++] = (IDX)

#define EMIT3(BC, IDX, CTX) \
    mgenc->add_bytecode(BC);\
	mgenc->add_bytecode(IDX);\
	mgenc->add_bytecode(CTX)
    /*mgenc->bytecode[mgenc->bp++] = (BC); \
    mgenc->bytecode[mgenc->bp++] = (IDX); \
    mgenc->bytecode[mgenc->bp++] = (CTX)*/


void bytecode_generator::emit_HALT( MethodGenerationContext* mgenc )
{
    EMIT1(BC_HALT);
}

void bytecode_generator::emit_DUP( MethodGenerationContext* mgenc )
{
    EMIT1(BC_DUP);
}

void bytecode_generator::emit_PUSH_LOCAL( MethodGenerationContext* mgenc, int idx, int ctx )
{
    EMIT3(BC_PUSH_LOCAL, idx, ctx);
}

void bytecode_generator::emit_PUSH_ARGUMENT( MethodGenerationContext* mgenc, int idx, int ctx )
{
    EMIT3(BC_PUSH_ARGUMENT, idx, ctx);
}

void bytecode_generator::emit_PUSH_FIELD( MethodGenerationContext* mgenc, VMSymbol* field )
{
    EMIT2(BC_PUSH_FIELD, mgenc->find_literal_index((VMObject*)field));// SEND(mgenc->literals, indexOf, field));
}

void bytecode_generator::emit_PUSH_BLOCK( MethodGenerationContext* mgenc, VMMethod* block )
{
    EMIT2(BC_PUSH_BLOCK, mgenc->find_literal_index((VMObject*)block));//SEND(mgenc->literals, indexOf, block));
}

void bytecode_generator::emit_PUSH_CONSTANT( MethodGenerationContext* mgenc, VMObject* cst )
{
    EMIT2(BC_PUSH_CONSTANT, mgenc->find_literal_index(cst));// SEND(mgenc->literals, indexOf, cst));
}

void bytecode_generator::emit_PUSH_CONSTANT_String( MethodGenerationContext* mgenc, VMString* str )
{
   EMIT2(BC_PUSH_CONSTANT, mgenc->find_literal_index((VMObject*)str)); //SEND(mgenc->literals, indexOfString, string));
}

void bytecode_generator::emit_PUSH_GLOBAL( MethodGenerationContext* mgenc, VMSymbol* global )
{
    EMIT2(BC_PUSH_GLOBAL, mgenc->find_literal_index((VMObject*)global));//SEND(mgenc->literals, indexOf, global));
}

void bytecode_generator::emit_POP( MethodGenerationContext* mgenc )
{
    EMIT1(BC_POP);
}

void bytecode_generator::emit_POP_LOCAL( MethodGenerationContext* mgenc, int idx, int ctx )
{
    EMIT3(BC_POP_LOCAL, idx, ctx);
}

void bytecode_generator::emit_POP_ARGUMENT( MethodGenerationContext* mgenc, int idx, int ctx )
{
    EMIT3(BC_POP_ARGUMENT, idx, ctx);
}

void bytecode_generator::emit_POP_FIELD( MethodGenerationContext* mgenc, VMSymbol* field )
{
    EMIT2(BC_POP_FIELD, mgenc->find_literal_index((VMObject*)field));// SEND(mgenc->literals, indexOf, field));
}

void bytecode_generator::emit_SEND( MethodGenerationContext* mgenc, VMSymbol* msg )
{
    EMIT2(BC_SEND, mgenc->find_literal_index((VMObject*)msg));
}

void bytecode_generator::emit_SUPER_SEND( MethodGenerationContext* mgenc, VMSymbol* msg )
{
    EMIT2(BC_SUPER_SEND, mgenc->find_literal_index((VMObject*)msg));
}

void bytecode_generator::emit_RETURN_LOCAL( MethodGenerationContext* mgenc )
{
    EMIT1(BC_RETURN_LOCAL);
}

void bytecode_generator::emit_RETURN_NON_LOCAL( MethodGenerationContext* mgenc )
{
    EMIT1(BC_RETURN_NON_LOCAL);
}

