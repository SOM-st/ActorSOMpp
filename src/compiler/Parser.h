#pragma once

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

#include <stdio.h>

//#include <vmobjects/VMMethod.h>
//#include <vmobjects/VMSymbol.h>
//#include <vmobjects/VMPrimitive.h>
#include "../misc/defs.h"
//#include <vm/Universe.h>
//using namespace std;
//#include "GenerationContexts.h"


typedef enum {
    NONE, Integer, Not, And, Or, Star, Div, Mod, Plus,
    Minus, Equal, More, Less, Comma, At, Per, NewBlock,
    EndBlock, Colon, Period, Exit, Assign, NewTerm, EndTerm, Pound,
    Primitive, Separator, STString, Identifier, Keyword, KeywordSequence,
    OperatorSequence
} Symbol;
static char* symnames[] = {
    "NONE", "Integer", "Not", "And", "Or", "Star", "Div", "Mod", "Plus",
    "Minus", "Equal", "More", "Less", "Comma", "At", "Per", "NewBlock",
    "EndBlock", "Colon", "Period", "Exit", "Assign", "NewTerm", "EndTerm",
    "Pound", "Primitive", "Separator", "STString", "Identifier", "Keyword",
    "KeywordSequence", "OperatorSequence"
};

class Parser
{
public:
	Parser(const FILE* fp);
	~Parser();
	//void init(const FILE* fp);
	//void init_string(pString str);

	void Classdef(class_generation_context* cgenc);
private:
	void fillbuffer(void);
	bool eob(void);
	void skipWhiteSpace(void);
	void skipComment(void);
	void getsym(void);
	void peek(void);
	bool symIn(Symbol* ss);
	bool accept(Symbol s);
	bool acceptOneOf(Symbol* ss);
	bool expect(Symbol s);
	bool expectOneOf(Symbol* ss);
	void SingleOperator(void);
	void instanceFields(class_generation_context* cgenc);
	void classFields(class_generation_context* cgenc);
	void method(method_generation_context* mgenc);
	void primitiveBlock(void);
	void pattern(method_generation_context* mgenc);
	void unaryPattern(method_generation_context* mgenc);
	void binaryPattern(method_generation_context* mgenc);
	void keywordPattern(method_generation_context* mgenc);
	void methodBlock(method_generation_context* mgenc);
	pVMSymbol unarySelector(void);
	pVMSymbol binarySelector(void);
	pString identifier(void);
	pString keyword(void);
	pString argument(void);
	void blockContents(method_generation_context* mgenc);
	void locals(method_generation_context* mgenc);
	void blockBody(method_generation_context* mgenc, bool seen_period);
	void result(method_generation_context* mgenc);
	void expression(method_generation_context* mgenc);
	void assignation(method_generation_context* mgenc);
	void assignments(method_generation_context* mgenc, list<string> l);
	pString assignment(method_generation_context* mgenc);
	void evaluation(method_generation_context* mgenc);
	void primary(method_generation_context* mgenc, bool* super);
	pString variable(void);
	void messages(method_generation_context* mgenc, bool super);
	void unaryMessage(method_generation_context* mgenc, bool super);
	void binaryMessage(method_generation_context* mgenc, bool super);
	void binaryOperand(method_generation_context* mgenc, bool* super);
	void keywordMessage(method_generation_context* mgenc, bool super);
	void formula(method_generation_context* mgenc);
	void nestedTerm(method_generation_context* mgenc);
	void literal(method_generation_context* mgenc);
	void literalNumber(method_generation_context* mgenc);
	uint32_t literalDecimal(void);
	int32_t negativeDecimal(void);
	uint32_t literalInteger(void);
	void literalSymbol(method_generation_context* mgenc);
	void literalString(method_generation_context* mgenc);
	pVMSymbol selector(void);
	pVMSymbol keywordSelector(void);
	pString string(void);
	void nestedBlock(method_generation_context* mgenc);
	void blockPattern(method_generation_context* mgenc);
	void blockArguments(method_generation_context* mgenc);
	void gen_push_variable(method_generation_context* mgenc, pString var);
	void gen_pop_variable(method_generation_context* mgenc, pString var);
	
//private fields
	FILE* infile;

	Symbol sym;
	char symc;
	char text[BUFSIZ];

	bool peekDone;
	Symbol nextSym;
	char nextSymc;
	char nextText[BUFSIZ];

	/**
	 * input buffer
	 */

	char buf[BUFSIZ];
	int bufp;

	/*static Symbol singleOpSyms[];
	static Symbol binaryOpSyms[];
	static Symbol keywordSelectorSyms[];*/
};
