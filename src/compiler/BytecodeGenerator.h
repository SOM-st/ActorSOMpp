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

class BytecodeGenerator {
public:
	void EmitHALT(MethodGenerationContext* mgenc);
	void EmitDUP(MethodGenerationContext* mgenc);
	void EmitPUSHLOCAL(MethodGenerationContext* mgenc, int idx, int ctx);
	void EmitPUSHARGUMENT(MethodGenerationContext* mgenc, int idx, int ctx);
	void EmitPUSHFIELD(MethodGenerationContext* mgenc, pVMSymbol field);
	void EmitPUSHBLOCK(MethodGenerationContext* mgenc, pVMMethod block);
	void EmitPUSHCONSTANT(MethodGenerationContext* mgenc, pVMObject cst);
	void EmitPUSHCONSTANTString(
        MethodGenerationContext* mgenc, pVMString str);
	void EmitPUSHGLOBAL(MethodGenerationContext* mgenc, pVMSymbol global);
	void EmitPOP(MethodGenerationContext* mgenc);
	void EmitPOPLOCAL(MethodGenerationContext* mgenc, int idx, int ctx);
	void EmitPOPARGUMENT(MethodGenerationContext* mgenc, int idx, int ctx);
	void EmitPOPFIELD(MethodGenerationContext* mgenc, pVMSymbol field);
	void EmitSEND(MethodGenerationContext* mgenc, pVMSymbol msg);
	void EmitSUPERSEND(MethodGenerationContext* mgenc, pVMSymbol msg);
	void EmitRETURNLOCAL(MethodGenerationContext* mgenc);
	void EmitRETURNNONLOCAL(MethodGenerationContext* mgenc);
};

#endif
