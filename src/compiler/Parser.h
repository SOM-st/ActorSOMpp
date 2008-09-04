#pragma once
#ifndef PARSER_H_
#define PARSER_H_

/*
 * $Id: Parser.h 143 2007-11-19 21:00:42Z michael.haupt $
 *
Copyright (c) 2007 Michael Haupt, Tobias Pape
Software Architecture Group, Hasso Plattner Institute, Potsdam, Germany
http://www.hpi.uni-potsdam.de/swa/

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
  */

//#include <stdio.h>
#include <fstream>
#include <string>
//#include <vmobjects/VMMethod.h>
//#include <vmobjects/VMSymbol.h>
//#include <vmobjects/VMPrimitive.h>
#include "../misc/defs.h"
#include "Lexer.h"
#include "ClassGenerationContext.h"
#include "MethodGenerationContext.h"
#include "BytecodeGenerator.h"
//#include <vm/Universe.h>
//using namespace std;
//#include "GenerationContexts.h"




class Parser
{
public:
	Parser(istream& file);
    Parser(const pString& stream);
	~Parser();
	//void init(const FILE* fp);
	//void init_string(pString str);

	void Classdef(class_generation_context* cgenc);
private:
	
	bool eob(void);
	
	bool symIn(Symbol* ss);
	bool accept(Symbol s);
	bool acceptOneOf(Symbol* ss);
	bool expect(Symbol s);
	bool expectOneOf(Symbol* ss);
	void SingleOperator(void);
	void instanceFields(class_generation_context* cgenc);
	void classFields(class_generation_context* cgenc);
	void method(MethodGenerationContext* mgenc);
	void primitiveBlock(void);
	void pattern(MethodGenerationContext* mgenc);
	void unaryPattern(MethodGenerationContext* mgenc);
	void binaryPattern(MethodGenerationContext* mgenc);
	void keywordPattern(MethodGenerationContext* mgenc);
	void methodBlock(MethodGenerationContext* mgenc);
	VMSymbol* unarySelector(void);
	VMSymbol* binarySelector(void);
	pString identifier(void);
	pString keyword(void);
	pString argument(void);
	void blockContents(MethodGenerationContext* mgenc);
	void locals(MethodGenerationContext* mgenc);
	void blockBody(MethodGenerationContext* mgenc, bool seen_period);
	void result(MethodGenerationContext* mgenc);
	void expression(MethodGenerationContext* mgenc);
	void assignation(MethodGenerationContext* mgenc);
	void assignments(MethodGenerationContext* mgenc, list<string> l);
	pString assignment(MethodGenerationContext* mgenc);
	void evaluation(MethodGenerationContext* mgenc);
	void primary(MethodGenerationContext* mgenc, bool* super);
	pString variable(void);
	void messages(MethodGenerationContext* mgenc, bool super);
	void unaryMessage(MethodGenerationContext* mgenc, bool super);
	void binaryMessage(MethodGenerationContext* mgenc, bool super);
	void binaryOperand(MethodGenerationContext* mgenc, bool* super);
	void keywordMessage(MethodGenerationContext* mgenc, bool super);
	void formula(MethodGenerationContext* mgenc);
	void nestedTerm(MethodGenerationContext* mgenc);
	void literal(MethodGenerationContext* mgenc);
	void literalNumber(MethodGenerationContext* mgenc);
	uint32_t literalDecimal(void);
	int32_t negativeDecimal(void);
	uint32_t literalInteger(void);
	void literalSymbol(MethodGenerationContext* mgenc);
	void literalString(MethodGenerationContext* mgenc);
	VMSymbol* selector(void);
	VMSymbol* keywordSelector(void);
	pString string(void);
	void nestedBlock(MethodGenerationContext* mgenc);
	void blockPattern(MethodGenerationContext* mgenc);
	void blockArguments(MethodGenerationContext* mgenc);
	void gen_push_variable(MethodGenerationContext* mgenc, pString var);
	void gen_pop_variable(MethodGenerationContext* mgenc, pString var);
	
//private fields
	Lexer* lexer;
	//ifstream& infile;

	Symbol sym;
	//char symc;
	//char text[BUFSIZ];
	pString text;

	//bool peekDone;
	Symbol nextSym;
	//char nextSymc;
	//char nextText[BUFSIZ];
	pString nextText;
	/**
	 * input buffer
	 */

	//char buf[BUFSIZ];
	/*std::string buf;
	unsigned int bufp;*/

	/*static Symbol singleOpSyms[];
	static Symbol binaryOpSyms[];
	static Symbol keywordSelectorSyms[];*/
    bytecode_generator* bcGen;
};

#endif
