/*
 * $Id: Parser.c 378 2008-05-17 00:46:39Z arne.bergmann $
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

#include "Parser.h"

//#include "GenerationContexts.h"
//#include "BytecodeGeneration.h"
#include "../vmobjects/VMMethod.h"
#include <iostream>
#include <cctype>
#include <sstream>
#include "BytecodeGenerator.h"
#include "../vmobjects/VMSymbol.h"
#include "../vm/Universe.h"
#include <stdlib.h>
#include <string.h>
//#include <memory/gc.h>
//#include <vmobjects/VMString.h>


#define GETSYM sym = lexer->getsym(); \
			   text = lexer->gettext()

#define PEEK nextSym = lexer->peek(); \
			 nextText = lexer->getnexttext()

Parser::Parser(istream& file){
	//lexer(file);
    sym = NONE;
    lexer = new Lexer(file);
    bcGen = new bytecode_generator();
    nextSym = NONE;
    //bufp = 0;

    GETSYM;
}

/*
bool Parser::eob()
{
    // TODO
}

void Parser::SingleOperator()
{
    // TODO
}

void Parser::assignments( MethodGenerationContext* mgenc, list<pString> l )
{
    // TODO
}

*/

Parser::~Parser() 
{
    delete(lexer);
    delete(bcGen);
    //if (lexer) delete(lexer);
    //if (bcGen) delete(bcGen);
}

//
// parsing
//


bool Parser::symIn(Symbol* ss) {
    while(*ss)
        if(*ss++ == sym)
            return true;
    return false;
}


bool Parser::accept(Symbol s) {
    if(sym == s) {
        GETSYM;
        return true;
    }
    return false;
}


bool Parser::acceptOneOf(Symbol* ss) {
    if(symIn(ss)) {
        GETSYM;
        return true;
    }
    return false;
}


#define _PRINTABLE_SYM (sym == Integer || sym >= STString)


bool Parser::expect(Symbol s) {
    if(accept(s))
        return true;
    fprintf(stderr, "Error: unexpected symbol. Expected %s, but found %s", 
            symnames[s], symnames[sym]);
    if(_PRINTABLE_SYM)
        fprintf(stderr, " (%s)", text.c_str());
	fprintf(stderr, ": %s\n", lexer->getrawbuffer().c_str());
    return false;
}


bool Parser::expectOneOf(Symbol* ss) {
    if(acceptOneOf(ss))
        return true;
    fprintf(stderr, "Error: unexpected symbol. Expected one of ");
    while(*ss)
        fprintf(stderr, "%s, ", symnames[*ss++]);
    fprintf(stderr, "but found %s", symnames[sym]);
    if(_PRINTABLE_SYM)
        fprintf(stderr, " (%s)", text.c_str());
	fprintf(stderr, ": %s\n", lexer->getrawbuffer().c_str());
    return false;
}




void Parser::gen_push_variable(MethodGenerationContext* mgenc, const pString& var) {
    // The purpose of this function is to find out whether the variable to be
    // pushed on the stack is a local variable, argument, or object field. This
    // is done by examining all available lexical contexts, starting with the
    // innermost (i.e., the one represented by mgenc).
    int index = 0;
    int context = 0;
    bool is_argument = false;
    
    if(mgenc->find_var(var, &index, &context, &is_argument)) {
		if(is_argument) 
            bcGen->emit_PUSH_ARGUMENT(mgenc, index, context);
        else 
            bcGen->emit_PUSH_LOCAL(mgenc, index, context);
    } else if(mgenc->find_field(var)) {
        VMSymbol* fieldName = _UNIVERSE->symbol_for(var);
		mgenc->add_literal_if_absent((VMObject*)fieldName);
        bcGen->emit_PUSH_FIELD(mgenc, fieldName);
		//SEND(mgenc->literals, addIfAbsent, fieldName);
        //bytecode_generator::emit_PUSH_FIELD(mgenc, fieldName);
    } else {
        
        VMSymbol* global = _UNIVERSE->symbol_for(var);
		mgenc->add_literal_if_absent((VMObject*)global);
        //SEND(mgenc->literals, addIfAbsent, global);
        bcGen->emit_PUSH_GLOBAL(mgenc, global);
    }
}


void Parser::gen_pop_variable(MethodGenerationContext* mgenc, const pString& var) {
    // The purpose of this function is to find out whether the variable to be
    // popped off the stack is a local variable, argument, or object field. This
    // is done by examining all available lexical contexts, starting with the
    // innermost (i.e., the one represented by mgenc).
    int index = 0;
    int context = 0;
    bool is_argument = false;
	//cout << "emit pop arg/local/field" << endl;
    if(mgenc->find_var(var, &index, &context, &is_argument)) {
        if(is_argument) bcGen->emit_POP_ARGUMENT(mgenc, index, context);
        else bcGen->emit_POP_LOCAL(mgenc, index, context);
    } else bcGen->emit_POP_FIELD(mgenc, _UNIVERSE->symbol_for(var));
}


//
// grammar
//


Symbol singleOpSyms[] = {
    Not, And, Or, Star, Div, Mod, Plus, Equal, More, Less, Comma, At, Per, NONE
};


Symbol binaryOpSyms[] = {
    Or, Comma, Minus, Equal, Not, And, Or, Star, Div, Mod, Plus, Equal,
    More, Less, Comma, At, Per, NONE
};


Symbol keywordSelectorSyms[] = { Keyword, KeywordSequence };




void Parser::Classdef(class_generation_context* cgenc) {
//    cgenc->name = Universe_symbol_for_chars(text);
	cgenc->set_name(_UNIVERSE->symbol_for(text));
    expect(Identifier);
    
    expect(Equal);
    
    if(sym == Identifier) {
       // cgenc->super_name = Universe_symbol_for_chars(text);
		cgenc->set_super_name(_UNIVERSE->symbol_for(text));
        accept(Identifier);
    } else cgenc->set_super_name(_UNIVERSE->symbol_for("Object"));
       // cgenc->super_name = Universe_symbol_for_chars("Object");
    
    expect(NewTerm);
    instanceFields(cgenc);
    while(sym == Identifier || sym == Keyword || sym == OperatorSequence ||
        symIn(binaryOpSyms)
    ) {
		
        MethodGenerationContext* mgenc = new MethodGenerationContext();
		mgenc->set_holder(cgenc);
		mgenc->add_argument("self");
    
        method(mgenc);
        
		if(mgenc->is_primitive())
            cgenc->add_instance_method((VMObject*)mgenc->AssemblePrimitive());
            //SEND(cgenc->instance_methods, add, VMPrimitive_assemble(&mgenc));
        else
			cgenc->add_instance_method((VMObject*)mgenc->Assemble());
            //SEND(cgenc->instance_methods, add, VMMethod_assemble(&mgenc));
        delete(mgenc);
        //method_genc_release(&mgenc);
    }
    
    if(accept(Separator)) {
        cgenc->set_class_side(true);
        classFields(cgenc);
        while(sym == Identifier || sym == Keyword || sym == OperatorSequence ||
            symIn(binaryOpSyms)
        ) {
            MethodGenerationContext* mgenc = new MethodGenerationContext();
			mgenc->set_holder(cgenc);
			mgenc->add_argument("self");
            //method_genc_init(&mgenc);
            //mgenc.holder_genc = cgenc;
            //
            //// each method has a self variable
            //SEND(mgenc.arguments, add, String_new("self"));
            //
            method(mgenc);
            
			if(mgenc->is_primitive())
                cgenc->add_class_method((VMObject*)mgenc->AssemblePrimitive());
                //SEND(cgenc->class_methods, add, VMPrimitive_assemble(&mgenc));
            else
				cgenc->add_class_method((VMObject*)mgenc->Assemble());
                //SEND(cgenc->class_methods, add, VMMethod_assemble(&mgenc));
            delete(mgenc);
            //method_genc_release(&mgenc);
        }    
    }
    expect(EndTerm);
}


void Parser::instanceFields(class_generation_context* cgenc) {
    if(accept(Or)) {
        while(sym == Identifier) {
            pString var = variable();
            cgenc->add_instance_field((VMObject*)_UNIVERSE->symbol_for(var));
            //SEND(cgenc->instance_fields, add, Universe_symbol_for(var));
            //SEND(var, free);
        }
        expect(Or);
    }
}


void Parser::classFields(class_generation_context* cgenc) {
    if(accept(Or)) {
        while(sym == Identifier) {
            pString var = variable();
			cgenc->add_class_field((VMObject*)_UNIVERSE->symbol_for(var));
            //SEND(cgenc->class_fields, add, Universe_symbol_for(var));
            //SEND(var, free);
        }
        expect(Or);
    }
}


void Parser::method(MethodGenerationContext* mgenc) {
    pattern(mgenc);
    
    expect(Equal);
    if(sym == Primitive) {
		mgenc->set_primitive(true);
        primitiveBlock();
    } else
        methodBlock(mgenc);
}


void Parser::primitiveBlock(void) {
    expect(Primitive);
}


void Parser::pattern(MethodGenerationContext* mgenc) {
    switch(sym) {
        case Identifier: 
            unaryPattern(mgenc);
            break;
        case Keyword: 
            keywordPattern(mgenc);
            break;
        default: 
            binaryPattern(mgenc);
            break;
    }
}


void Parser::unaryPattern(MethodGenerationContext* mgenc) {
    mgenc->set_signature(unarySelector());
}


void Parser::binaryPattern(MethodGenerationContext* mgenc) {
    mgenc->set_signature(binarySelector());
	mgenc->add_argument_if_absent(argument());
    //SEND(mgenc->arguments, addStringIfAbsent, argument());
	//argument();
}


void Parser::keywordPattern(MethodGenerationContext* mgenc) {
    pString kw;// = String_new("");
    do {
        kw.append(keyword());// kw = SEND(kw, concat, keyword());
		mgenc->add_argument_if_absent(argument());
        //SEND(mgenc->arguments, addStringIfAbsent, argument());
		//argument();
    } while(sym == Keyword);
    
    mgenc->set_signature(_UNIVERSE->symbol_for(kw));// Universe_symbol_for(kw);
    //SEND(kw, free);
}


void Parser::methodBlock(MethodGenerationContext* mgenc) {
    expect(NewTerm);
    blockContents(mgenc);
    // if no return has been generated so far, we can be sure there was no .
    // terminating the last expression, so the last expression's value must be
    // popped off the stack and a ^self be generated
    if(!mgenc->is_finished()) {
        bcGen->emit_POP(mgenc);
        bcGen->emit_PUSH_ARGUMENT(mgenc, 0, 0);
        bcGen->emit_RETURN_LOCAL(mgenc);
        mgenc->set_finished();
    }
    
    expect(EndTerm);
}


VMSymbol* Parser::unarySelector(void) {
    return _UNIVERSE->symbol_for(identifier());
}


VMSymbol* Parser::binarySelector(void) {
    pString s(text);
    
    if(accept(Or))
        ;
    else if(accept(Comma))
        ;
    else if(accept(Minus))
        ;
    else if(accept(Equal))
        ;
    else if(acceptOneOf(singleOpSyms))
        ;
    else if(accept(OperatorSequence))
        ;
    else
        expect(NONE);
    
    VMSymbol* symb = _UNIVERSE->symbol_for(s);
    //SEND(s, free);
    return symb;
}


pString Parser::identifier(void) {
    pString s(text);
    if(accept(Primitive))
        ; // text is set
    else
        expect(Identifier);
    
    return s;
}


pString Parser::keyword(void) {
    pString s(text);
    expect(Keyword);
    
    return s;
}


pString Parser::argument(void) {
    return variable();
}


void Parser::blockContents(MethodGenerationContext* mgenc) {
    if(accept(Or)) {
        locals(mgenc);
        expect(Or);
    }
    blockBody(mgenc, false);
}


void Parser::locals(MethodGenerationContext* mgenc) {
    while(sym == Identifier) //variable();
		mgenc->add_local_if_absent(variable());
        //SEND(mgenc->locals, addStringIfAbsent, variable());
}


void Parser::blockBody(MethodGenerationContext* mgenc, bool seen_period) {
    if(accept(Exit))
        result(mgenc);
    else if(sym == EndBlock) {
		if(seen_period) {
            // a POP has been generated which must be elided (blocks always
            // return the value of the last expression, regardless of whether it
            // was terminated with a . or not)
            mgenc->RemoveLastBytecode();
		}
        bcGen->emit_RETURN_LOCAL(mgenc);
		//cout << "emit return local" << endl;
//        mgenc->finished = true;
		mgenc->set_finished();
    } else if(sym == EndTerm) {
        if(seen_period)
            // we can be sure it's a method we're parsing, not a block, so the
            // PUSH_ARGUMENT can be created immediately
            bcGen->emit_PUSH_ARGUMENT(mgenc, 0, 0);
		bcGen->emit_RETURN_LOCAL(mgenc);
		mgenc->set_finished();
//      mgenc->finished = true;
    } else {
        expression(mgenc);
        if(accept(Period)) {
            bcGen->emit_POP(mgenc);
			//cout << "emit pop" << endl;
            blockBody(mgenc, true);
        }
    }
}


void Parser::result(MethodGenerationContext* mgenc) {
    expression(mgenc);
	//cout << "emit return" << endl;
	if(mgenc->is_block_method()) bcGen->emit_RETURN_NON_LOCAL(mgenc);
	else bcGen->emit_RETURN_LOCAL(mgenc);
    //mgenc->finished = true;
    mgenc->set_finished(true);
	accept(Period);
}


void Parser::expression(MethodGenerationContext* mgenc) {
    PEEK;//peek();
    if(nextSym == Assign)
        assignation(mgenc);
    else
        evaluation(mgenc);
}


void Parser::assignation(MethodGenerationContext* mgenc) {
	list<pString> l;
    //cout << "assignation" << endl;
    assignments(mgenc, l);
    evaluation(mgenc);
    list<pString>::iterator i;
    for(i=l.begin(); i != l.end(); ++i)
        bcGen->emit_DUP(mgenc);
    for(i=l.begin(); i != l.end(); ++i)//SEND(l, size); i++)
        gen_pop_variable(mgenc, (*i));//SEND(l, get, i));
    
    //SEND(l, deep_free);
}


void Parser::assignments(MethodGenerationContext* mgenc, list<pString>& l) {
    if(sym == Identifier) {
        l.push_back(assignment(mgenc));//SEND(l, add, assignment(mgenc));
        PEEK;//peek();
        if(nextSym == Assign)
            assignments(mgenc, l);
    }
}


pString Parser::assignment(MethodGenerationContext* mgenc) {
    pString v = variable();
    VMSymbol* var = _UNIVERSE->symbol_for(v);
	mgenc->add_literal_if_absent((VMObject*)var);
    //SEND(mgenc->literals, addIfAbsent, var);
    
    expect(Assign);
    
    return v;
}


void Parser::evaluation(MethodGenerationContext* mgenc) {
    bool super;
    primary(mgenc, &super);
    if(sym == Identifier || sym == Keyword || sym == OperatorSequence ||
        symIn(binaryOpSyms)
    ) {       
        messages(mgenc, super);
    }
}


void Parser::primary(MethodGenerationContext* mgenc, bool* super) {
    *super = false;
    switch(sym) {
        case Identifier: {
            pString v = variable();
			if(v == pString("super")) { //SEND(v, compareToChars, "super") == 0) {
                *super = true;
                // sends to super push self as the receiver
                //SEND(v, free);
                v = pString("self");//String_new("self");
            }
            
            gen_push_variable(mgenc, v);
            //SEND(v, free);
            break;
        }
        case NewTerm:
            nestedTerm(mgenc);
            break;
        case NewBlock: {
            MethodGenerationContext* bgenc = new MethodGenerationContext();
			bgenc->set_is_block_method(true);
			bgenc->set_holder(mgenc->get_holder());
			bgenc->set_outer(mgenc);
            /*method_genc_init(&bgenc);
            bgenc.block_method = true;
            bgenc.holder_genc = mgenc->holder_genc;
            bgenc.outer_genc = mgenc;*/
            
            nestedBlock(bgenc);
            
            VMMethod* block_method = bgenc->Assemble();
			mgenc->add_literal(block_method);
            //SEND(mgenc->literals, add, block_method)
            bcGen->emit_PUSH_BLOCK(mgenc, block_method);
			//cout << "emit push block" << endl;
			delete(bgenc);
            //method_genc_release(&bgenc);
            break;
        }
        default:
            literal(mgenc);
            break;
    }
}


pString Parser::variable(void) {
    return identifier();
}


void Parser::messages(MethodGenerationContext* mgenc, bool super) {
    if(sym == Identifier) {
        do {
            // only the first message in a sequence can be a super send
            unaryMessage(mgenc, super);
            super = false;
        } while(sym == Identifier);
        
        while(sym == OperatorSequence || symIn(binaryOpSyms)) {
            binaryMessage(mgenc, false);
        }
        
        if(sym == Keyword) {
            keywordMessage(mgenc, false);
        }
    } else if(sym == OperatorSequence || symIn(binaryOpSyms)) {
        do {
            // only the first message in a sequence can be a super send
            binaryMessage(mgenc, super);
            super = false;
        } while(sym == OperatorSequence || symIn(binaryOpSyms));
        
        if(sym == Keyword) {
            keywordMessage(mgenc, false);
        }
    } else
        keywordMessage(mgenc, super);
}


void Parser::unaryMessage(MethodGenerationContext* mgenc, bool super) {
    VMSymbol* msg = unarySelector();
	mgenc->add_literal_if_absent((VMObject*)msg);
    //SEND(mgenc->literals, addIfAbsent, msg);
    if(super) bcGen->emit_SUPER_SEND(mgenc, msg);
    else bcGen->emit_SEND(mgenc, msg);
	//cout << endl;
}


void Parser::binaryMessage(MethodGenerationContext* mgenc, bool super) {
    VMSymbol* msg = binarySelector();
	mgenc->add_literal_if_absent((VMObject*)msg);
    //SEND(mgenc->literals, addIfAbsent, msg);
    
    bool tmp_bool = false;
    binaryOperand(mgenc, &tmp_bool);
    
    if(super)
        bcGen->emit_SUPER_SEND(mgenc, msg);
    else
        bcGen->emit_SEND(mgenc, msg);
	//cout << endl;
}


void Parser::binaryOperand(MethodGenerationContext* mgenc, bool* super) {
    primary(mgenc, super);
    
    while(sym == Identifier)
        unaryMessage(mgenc, *super);
}


void Parser::keywordMessage(MethodGenerationContext* mgenc, bool super) {
    pString kw;
    do {
        kw.append(keyword());// = SEND(kw, concat, keyword());
        formula(mgenc);
    } while(sym == Keyword);
    
    VMSymbol* msg = _UNIVERSE->symbol_for(kw);
    //SEND(kw, free);
	mgenc->add_literal_if_absent((VMObject*)msg);
    //SEND(mgenc->literals, addIfAbsent, msg);
    
    if(super) bcGen->emit_SUPER_SEND(mgenc, msg);
    else bcGen->emit_SEND(mgenc, msg);
	//cout << endl;
}


void Parser::formula(MethodGenerationContext* mgenc) {
    bool super;
    binaryOperand(mgenc, &super);
    
    // only the first message in a sequence can be a super send
    if(sym == OperatorSequence || symIn(binaryOpSyms))
        binaryMessage(mgenc, super);
    while(sym == OperatorSequence || symIn(binaryOpSyms))
        binaryMessage(mgenc, false);
}


void Parser::nestedTerm(MethodGenerationContext* mgenc) {
    expect(NewTerm);
    expression(mgenc);
    expect(EndTerm);
}


void Parser::literal(MethodGenerationContext* mgenc) {
    switch(sym) {
        case Pound: 
            literalSymbol(mgenc);
            break;
        case STString: 
            literalString(mgenc);
            break;
        default: 
            literalNumber(mgenc);
            break;
    }
}


void Parser::literalNumber(MethodGenerationContext* mgenc) {
    int32_t val;
    if(sym == Minus)
        val = negativeDecimal();
    else
        val = literalDecimal();
    
    VMInteger* literal = _UNIVERSE->new_integer(val);
	/*stringstream s;
	pString str_lit = "";
	s << literal->GetEmbeddedInteger();
	str_lit += s.str();*/
	mgenc->add_literal_if_absent((VMObject*)literal);
    //SEND(mgenc->literals, addIfAbsent, literal);
    bcGen->emit_PUSH_CONSTANT(mgenc, (VMObject*)literal);
	//cout << "emit push constant literal number" << endl;
}


uint32_t Parser::literalDecimal(void) {
    return literalInteger();
}


int32_t Parser::negativeDecimal(void) {
    expect(Minus);
    return  -((int32_t)literalInteger());
}


uint32_t Parser::literalInteger(void) {
    uint32_t i = (uint32_t) strtoul(text.c_str(), NULL, 10);
    expect(Integer);
    return i;
}


void Parser::literalSymbol(MethodGenerationContext* mgenc) {
    VMSymbol* symb;
    expect(Pound);
    if(sym == STString) {
        pString s = _string();
        symb = _UNIVERSE->symbol_for(s);
        //SEND(s, free);
    } else
        symb = selector();
	mgenc->add_literal_if_absent((VMObject*)symb);
    //SEND(mgenc->literals, addIfAbsent, symb);
    //cout << "emit push constant literal symbol" << endl;
    bcGen->emit_PUSH_CONSTANT(mgenc, (VMObject*)symb);
}


void Parser::literalString(MethodGenerationContext* mgenc) {
    pString s = _string();
	
    VMString* str = _UNIVERSE->new_string(s);
    mgenc->add_literal_if_absent((VMObject*)str);
    //SEND(s, free);
    //
    //SEND(mgenc->literals, addIfAbsent, str);
    bcGen->emit_PUSH_CONSTANT(mgenc,(VMObject*)str);
    //cout << "emit push constant literal string" << endl;
}


VMSymbol* Parser::selector(void) {
    if(sym == OperatorSequence || symIn(singleOpSyms))
        return binarySelector();
    else if(sym == Keyword || sym == KeywordSequence)
        return keywordSelector();
    else
        return unarySelector();
}


VMSymbol* Parser::keywordSelector(void) {
    pString s(text);// = String_new(text);
    expectOneOf(keywordSelectorSyms);
    VMSymbol* symb = _UNIVERSE->symbol_for(s);
    //SEND(s, free);
    return symb;
}


pString Parser::_string(void) {
    pString s(text); 
    expect(STString);    
    return s; // <-- Literal strings are At Most BUFSIZ chars long.
}


void Parser::nestedBlock(MethodGenerationContext* mgenc) {
    #define BLOCK_METHOD_S "$block method"
    #define BLOCK_METHOD_LEN (13)
	mgenc->add_argument_if_absent("$block self");
    //SEND(mgenc->arguments, addStringIfAbsent, String_new("$block self"));
    
    expect(NewBlock);
    if(sym == Colon)
        blockPattern(mgenc);
    
    // generate Block signature
    pString block_sig = pString(BLOCK_METHOD_S);// String_new(BLOCK_METHOD_S);
	size_t arg_size = mgenc->get_number_of_arguments();// SEND(mgenc->arguments, size);
    for(size_t i = 1; i < arg_size; i++)
		block_sig += ":";//.append(':');
        //SEND(block_sig, concatChar, ':');

    mgenc->set_signature(_UNIVERSE->symbol_for(block_sig));
    
    blockContents(mgenc);
    
    // if no return has been generated, we can be sure that the last expression
    // in the block was not terminated by ., and can generate a return
    if(!mgenc->is_finished()) {
		bcGen->emit_RETURN_LOCAL(mgenc);
        mgenc->set_finished(true);
    }
    
    expect(EndBlock);
}


void Parser::blockPattern(MethodGenerationContext* mgenc) {
    blockArguments(mgenc);
    expect(Or);
}


void Parser::blockArguments(MethodGenerationContext* mgenc) {
    do {
        expect(Colon);
		mgenc->add_argument_if_absent(argument());
        //SEND(mgenc->arguments, addStringIfAbsent, argument());
		//argument();
    } while(sym == Colon);
}


