#pragma once
#ifndef METHODGENERATIONCONTEXT_H_
#define METHODGeNERATIONCONTEXT_H_

#include "../misc/defs.h"
#include "ClassGenerationContext.h"
#include "../misc/ExtendedVector.h"
#include "GenerationContextCore.h"
#include "../misc/ExtendedList.h"
class VMMethod;
class VMArray;
class VMPrimitive;

class MethodGenerationContext : public generation_context_core {
public:
	MethodGenerationContext();
	~MethodGenerationContext();
    
    VMMethod* Assemble();
    VMPrimitive* AssemblePrimitive();

	int8_t find_literal_index(VMObject* lit);//pVMObject lit);
	bool find_var(const pString& var, int* index, int* context, bool* is_argument);
	bool find_field(const pString& field);
	uint8_t compute_stack_depth();

	void set_holder(class_generation_context* holder);
	void set_outer(MethodGenerationContext* outer);
	void set_is_block_method(bool is_block = true);
	void set_signature(VMSymbol* sig);
	void add_argument(const pString& arg);
	void set_primitive(bool prim = true);
	void add_local(const pString& local);
	void add_literal(VMObject* lit);
	bool add_argument_if_absent(const pString& arg);
	bool add_local_if_absent(const pString& local);
	bool add_literal_if_absent(VMObject* lit);
	void set_finished(bool finished = true);
	class_generation_context* get_holder();
	MethodGenerationContext* get_outer();
	VMSymbol* get_signature();
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
    VMSymbol*                  signature;
    ExtendedList<std::string>            arguments;
    bool                       primitive;
    ExtendedList<std::string>            locals;
    pList            literals;
    bool                       finished;
    int32_t                   bp;
	vector<uint8_t>            bytecode;
};

#endif
