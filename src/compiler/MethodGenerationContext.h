#pragma once
#ifndef METHODGENERATIONCONTEXT_H_
#define METHODGeNERATIONCONTEXT_H_

#include "../misc/defs.h"
#include "ClassGenerationContext.h"
#include "../misc/ExtendedVector.h"
#include "GenerationContextCore.h"

class MethodGenerationContext : public generation_context_core {
public:
	MethodGenerationContext();
	~MethodGenerationContext();

	int8_t find_literal_index(pVMSymbol lit);//pVMObject lit);
	bool find_var(pString var, int* index, int* context, bool* is_argument);
	bool find_field(pString field);
	uint8_t compute_stack_depth();

	void set_holder(class_generation_context* holder);
	void set_outer(MethodGenerationContext* outer);
	void set_is_block_method(bool is_block = true);
	void set_signature(pVMSymbol sig);
	void add_argument(pString arg);
	void set_primitive(bool prim = true);
	void add_local(pString local);
	void add_literal(pString lit);
	bool add_argument_if_absent(pString arg);
	bool add_local_if_absent(pString local);
	bool add_literal_if_absent(pString lit);
	void set_finished(bool finished = true);
	class_generation_context* get_holder();
	MethodGenerationContext* get_outer();
	pVMSymbol get_signature();
	bool is_primitive();
	bool is_block_method();
	bool is_finished();
	void inc_bp() { bp++; };
	void dec_bp() { bp--; };
	int get_number_of_arguments();
	void add_bytecode(uint8_t bc);
private:
	class_generation_context*  holder_genc;
    MethodGenerationContext* outer_genc;
    bool                       block_method;
    pVMSymbol                  signature;
    pList            arguments;
    bool                       primitive;
    pList            locals;
    pList            literals;
    bool                       finished;
    uint32_t                   bp;
	vector<uint8_t>            bytecode;
};

#endif
