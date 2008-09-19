#pragma once
#ifndef BYTECODEGENERATOR_H_
#define BYTECODEGENERATOR_H_


#include "MethodGenerationContext.h"
#include "../interpreter/bytecodes.h"
#include "../misc/defs.h"

class VMMethod;
class VMSymbol;
class VMObject;
class VMString;

class bytecode_generator {
public:
	void emit_HALT(MethodGenerationContext* mgenc);
	void emit_DUP(MethodGenerationContext* mgenc);
	void emit_PUSH_LOCAL(MethodGenerationContext* mgenc, int idx, int ctx);
	void emit_PUSH_ARGUMENT(MethodGenerationContext* mgenc, int idx, int ctx);
	void emit_PUSH_FIELD(MethodGenerationContext* mgenc, VMSymbol* field);
	void emit_PUSH_BLOCK(MethodGenerationContext* mgenc, VMMethod* block);
	void emit_PUSH_CONSTANT(MethodGenerationContext* mgenc, VMObject* cst);
	void emit_PUSH_CONSTANT_String(
		MethodGenerationContext* mgenc,
		VMString* str
	);
	void emit_PUSH_GLOBAL(MethodGenerationContext* mgenc, VMSymbol* global);
	void emit_POP(MethodGenerationContext* mgenc);
	void emit_POP_LOCAL(MethodGenerationContext* mgenc, int idx, int ctx);
	void emit_POP_ARGUMENT(MethodGenerationContext* mgenc, int idx, int ctx);
	void emit_POP_FIELD(MethodGenerationContext* mgenc, VMSymbol* field);
	void emit_SEND(MethodGenerationContext* mgenc, VMSymbol* msg);
	void emit_SUPER_SEND(MethodGenerationContext* mgenc, VMSymbol* msg);
	void emit_RETURN_LOCAL(MethodGenerationContext* mgenc);
	void emit_RETURN_NON_LOCAL(MethodGenerationContext* mgenc);
};

#endif
