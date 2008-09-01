//#pragma once
//#ifndef BYTECODEGENERATOR_H_
//#define BYTECODEGENERATOR_H_
//#define EMIT1(BC) \
//	mgenc->add_bytecode(BC)
//    //mgenc->bytecode[mgenc->bp++] = (BC)
//
//#define EMIT2(BC, IDX) \
//	mgenc->add_bytecode(BC);\
//	mgenc->add_bytecode(IDX)
//    //mgenc->bytecode[mgenc->bp++] = (BC); \
//	//mgenc->bytecode[mgenc->bp++] = (IDX)
//
//#define EMIT3(BC, IDX, CTX) \
//	mgenc->add_bytecode(BC);\
//	mgenc->add_bytecode(IDX);\
//	mgenc->add_bytecode(CTX)
//    /*mgenc->bytecode[mgenc->bp++] = (BC); \
//    mgenc->bytecode[mgenc->bp++] = (IDX); \
//    mgenc->bytecode[mgenc->bp++] = (CTX)*/
//
//#include "MethodGenerationContext.h"
//#include "../interpreter/bytecodes.h"
//#include "../misc/defs.h"
//
//class bytecode_generator {
//public:
//	static void emit_HALT(MethodGenerationContext* mgenc) {
//		EMIT1(BC_HALT);
//	}
//
//
//	static void emit_DUP(MethodGenerationContext* mgenc) {
//		EMIT1(BC_DUP);
//	}
//
//
//	static void emit_PUSH_LOCAL(MethodGenerationContext* mgenc, int idx, int ctx) {
//		EMIT3(BC_PUSH_LOCAL, idx, ctx);
//	}
//
//
//	static void emit_PUSH_ARGUMENT(MethodGenerationContext* mgenc, int idx, int ctx) {
//		EMIT3(BC_PUSH_ARGUMENT, idx, ctx);
//	}
//
//
//	static void emit_PUSH_FIELD(MethodGenerationContext* mgenc, pVMSymbol field) {
//		EMIT2(BC_PUSH_FIELD, SEND(mgenc->literals, indexOf, field));
//	}
//
//
//	static void emit_PUSH_BLOCK(MethodGenerationContext* mgenc, pVMMethod block) {
//		EMIT2(BC_PUSH_BLOCK, SEND(mgenc->literals, indexOf, block));
//	}
//
//
//	static void emit_PUSH_CONSTANT(MethodGenerationContext* mgenc, pVMObject cst) {
//		EMIT2(BC_PUSH_CONSTANT, SEND(mgenc->literals, indexOf, cst));
//	}
//
//
//	static void emit_PUSH_CONSTANT_String(
//		MethodGenerationContext* mgenc,
//		pVMString str
//	) {
//		pString string = SEND(str, get_embedded_string);
//		EMIT2(BC_PUSH_CONSTANT, SEND(mgenc->literals, indexOfString, string));
//	}
//
//
//	static void emit_PUSH_GLOBAL(MethodGenerationContext* mgenc, pVMSymbol global) {
//		EMIT2(BC_PUSH_GLOBAL, SEND(mgenc->literals, indexOf, global));
//	}
//
//
//	static void emit_POP(MethodGenerationContext* mgenc) {
//		EMIT1(BC_POP);
//	}
//
//
//	static void emit_POP_LOCAL(MethodGenerationContext* mgenc, int idx, int ctx) {
//		EMIT3(BC_POP_LOCAL, idx, ctx);
//	}
//
//
//	static void emit_POP_ARGUMENT(MethodGenerationContext* mgenc, int idx, int ctx) {
//		EMIT3(BC_POP_ARGUMENT, idx, ctx);
//	}
//
//
//	static void emit_POP_FIELD(MethodGenerationContext* mgenc, pVMSymbol field) {
//		EMIT2(BC_POP_FIELD, SEND(mgenc->literals, indexOf, field));
//	}
//
//
//	static void emit_SEND(MethodGenerationContext* mgenc, pVMSymbol msg) {
//		EMIT2(BC_SEND, method_genc_find_literal_index(mgenc, (pVMObject)msg));
//	}
//
//
//	static void emit_SUPER_SEND(MethodGenerationContext* mgenc, pVMSymbol msg) {
//		EMIT2(BC_SUPER_SEND, method_genc_find_literal_index(mgenc, (pVMObject)msg));
//	}
//
//
//	static void emit_RETURN_LOCAL(MethodGenerationContext* mgenc) {
//		EMIT1(BC_RETURN_LOCAL);
//	}
//
//
//	static void emit_RETURN_NON_LOCAL(MethodGenerationContext* mgenc) {
//		EMIT1(BC_RETURN_NON_LOCAL);
//	}
//}
//
//#endif