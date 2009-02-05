#include "BytecodeGenerator.h"
#include "../vmobjects/VMObject.h"
#include "../vmobjects/VMMethod.h"


#define EMIT1(BC) \
    mgenc->AddBytecode(BC)


#define EMIT2(BC, IDX) \
    mgenc->AddBytecode(BC);\
	mgenc->AddBytecode(IDX)


#define EMIT3(BC, IDX, CTX) \
    mgenc->AddBytecode(BC);\
	mgenc->AddBytecode(IDX);\
	mgenc->AddBytecode(CTX)


void BytecodeGenerator::emit_HALT( MethodGenerationContext* mgenc ) {
    EMIT1(BC_HALT);
}


void BytecodeGenerator::emit_DUP( MethodGenerationContext* mgenc ) {
    EMIT1(BC_DUP);
}


void BytecodeGenerator::emit_PUSH_LOCAL(
                MethodGenerationContext* mgenc, int idx, int ctx ) {
    EMIT3(BC_PUSH_LOCAL, idx, ctx);
}


void BytecodeGenerator::emit_PUSH_ARGUMENT( 
                MethodGenerationContext* mgenc, int idx, int ctx ) {
    EMIT3(BC_PUSH_ARGUMENT, idx, ctx);
}


void BytecodeGenerator::emit_PUSH_FIELD(
                MethodGenerationContext* mgenc, pVMSymbol field ) {
    EMIT2(BC_PUSH_FIELD, mgenc->FindLiteralIndex((pVMObject)field));
}


void BytecodeGenerator::emit_PUSH_BLOCK(
                MethodGenerationContext* mgenc, pVMMethod block ) {
    EMIT2(BC_PUSH_BLOCK, mgenc->FindLiteralIndex((pVMObject)(block)));
}


void BytecodeGenerator::emit_PUSH_CONSTANT(
                MethodGenerationContext* mgenc, pVMObject cst ) {
    EMIT2(BC_PUSH_CONSTANT, mgenc->FindLiteralIndex(cst));
}


void BytecodeGenerator::emit_PUSH_CONSTANT_String( 
                MethodGenerationContext* mgenc, pVMString str ){
   EMIT2(BC_PUSH_CONSTANT, mgenc->FindLiteralIndex((pVMObject)str));
}


void BytecodeGenerator::emit_PUSH_GLOBAL(
                MethodGenerationContext* mgenc, pVMSymbol global ) {
    EMIT2(BC_PUSH_GLOBAL, mgenc->FindLiteralIndex((pVMObject)global));
}


void BytecodeGenerator::emit_POP( MethodGenerationContext* mgenc ) {
    EMIT1(BC_POP);
}


void BytecodeGenerator::emit_POP_LOCAL(
                MethodGenerationContext* mgenc, int idx, int ctx ) {
    EMIT3(BC_POP_LOCAL, idx, ctx);
}


void BytecodeGenerator::emit_POP_ARGUMENT(
                MethodGenerationContext* mgenc, int idx, int ctx ) {
    EMIT3(BC_POP_ARGUMENT, idx, ctx);
}


void BytecodeGenerator::emit_POP_FIELD(
                MethodGenerationContext* mgenc, pVMSymbol field ) {
    EMIT2(BC_POP_FIELD, mgenc->FindLiteralIndex((pVMObject)field));
}


void BytecodeGenerator::emit_SEND(
                MethodGenerationContext* mgenc, pVMSymbol msg ) {
    EMIT2(BC_SEND, mgenc->FindLiteralIndex((pVMObject)msg));
}


void BytecodeGenerator::emit_SUPER_SEND(
                MethodGenerationContext* mgenc, pVMSymbol msg ) {
    EMIT2(BC_SUPER_SEND, mgenc->FindLiteralIndex((pVMObject)msg));
}


void BytecodeGenerator::emit_RETURN_LOCAL(
                MethodGenerationContext* mgenc ) {
    EMIT1(BC_RETURN_LOCAL);
}


void BytecodeGenerator::emit_RETURN_NON_LOCAL(
                MethodGenerationContext* mgenc ) {
    EMIT1(BC_RETURN_NON_LOCAL);
}
