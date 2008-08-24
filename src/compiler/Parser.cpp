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

#include <iostream>
#include <cctype>
#include <sstream>
#include "BytecodeGenerator.h"
//#include <string>
//#include <memory/gc.h>
//#include <vmobjects/VMString.h>


#pragma mark Stream handling

#define GETSYM sym = lexer->getsym(); \
			   text = lexer->gettext()

#define PEEK nextSym = lexer->peek(); \
			 nextText = lexer->getnexttext()

Parser::Parser(ifstream& file){
	//lexer(file);
    sym = NONE;
    lexer = new Lexer(file);
    nextSym = NONE;
    //bufp = 0;

    GETSYM;
}

Parser::~Parser() {
}

//
// parsing
//

#pragma mark Parser helpers

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
        fprintf(stderr, " (%s)", text);
	fprintf(stderr, ": %s\n", lexer->getrawbuffer());
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
        fprintf(stderr, " (%s)", text);
	fprintf(stderr, ": %s\n", lexer->getrawbuffer());
    return false;
}


#pragma mark helper functions for pushing / popping variables


void Parser::gen_push_variable(method_generation_context* mgenc, pString var) {
    // The purpose of this function is to find out whether the variable to be
    // pushed on the stack is a local variable, argument, or object field. This
    // is done by examining all available lexical contexts, starting with the
    // innermost (i.e., the one represented by mgenc).
    int index = 0;
    int context = 0;
    bool is_argument = false;
	//cout << "emit push arg/local/field/global" << endl;
    if(mgenc->find_var(var, &index, &context, &is_argument))
		if(is_argument) ;
            //bytecode_generator::emit_PUSH_ARGUMENT(mgenc, index, context);
        else;
            //bytecode_generator::emit_PUSH_LOCAL(mgenc, index, context);
	else if(mgenc->find_field(var)) {
        pVMSymbol fieldName = var;//Universe_symbol_for(var);
		mgenc->add_literal_if_absent(fieldName);
		//SEND(mgenc->literals, addIfAbsent, fieldName);
        //bytecode_generator::emit_PUSH_FIELD(mgenc, fieldName);
    } else {
        pVMSymbol global = var;//Universe_symbol_for(var);
		mgenc->add_literal_if_absent(global);
        //SEND(mgenc->literals, addIfAbsent, global);
        //bytecode_generator::emit_PUSH_GLOBAL(mgenc, global);
    }
}


void Parser::gen_pop_variable(method_generation_context* mgenc, pString var) {
    // The purpose of this function is to find out whether the variable to be
    // popped off the stack is a local variable, argument, or object field. This
    // is done by examining all available lexical contexts, starting with the
    // innermost (i.e., the one represented by mgenc).
    int index = 0;
    int context = 0;
    bool is_argument = false;
	//cout << "emit pop arg/local/field" << endl;
	if(mgenc->find_var(var, &index, &context, &is_argument))
        if(is_argument);
            //bytecode_generator::emit_POP_ARGUMENT(mgenc, index, context);
        else;
            //bytecode_generator::emit_POP_LOCAL(mgenc, index, context);
    else;
        //bytecode_generator::emit_POP_FIELD(mgenc, Universe_symbol_for(var));
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


#pragma mark Parser grammar


void Parser::Classdef(class_generation_context* cgenc) {
//    cgenc->name = Universe_symbol_for_chars(text);
	cgenc->set_name(text);
    expect(Identifier);
    
    expect(Equal);
    
    if(sym == Identifier) {
       // cgenc->super_name = Universe_symbol_for_chars(text);
		cgenc->set_super_name(text);
        accept(Identifier);
    } else cgenc->set_super_name("Object");
       // cgenc->super_name = Universe_symbol_for_chars("Object");
    
    expect(NewTerm);
    instanceFields(cgenc);
    while(sym == Identifier || sym == Keyword || sym == OperatorSequence ||
        symIn(binaryOpSyms)
    ) {
		
        method_generation_context* mgenc = new method_generation_context();
		mgenc->set_holder(cgenc);
		mgenc->add_argument("self");
    
        method(mgenc);
        
		if(mgenc->is_primitive())
			cgenc->add_instance_method(mgenc->get_signature());
            //SEND(cgenc->instance_methods, add, VMPrimitive_assemble(&mgenc));
        else
			cgenc->add_instance_method(mgenc->get_signature());
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
            method_generation_context* mgenc = new method_generation_context();
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
				cgenc->add_class_method(mgenc->get_signature());
                //SEND(cgenc->class_methods, add, VMPrimitive_assemble(&mgenc));
            else
				cgenc->add_class_method(mgenc->get_signature());
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
			cgenc->add_instance_field(var);
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
			cgenc->add_class_field(var);
            //SEND(cgenc->class_fields, add, Universe_symbol_for(var));
            //SEND(var, free);
        }
        expect(Or);
    }
}


void Parser::method(method_generation_context* mgenc) {
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


void Parser::pattern(method_generation_context* mgenc) {
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


void Parser::unaryPattern(method_generation_context* mgenc) {
    mgenc->set_signature(unarySelector());
}


void Parser::binaryPattern(method_generation_context* mgenc) {
    mgenc->set_signature(binarySelector());
	mgenc->add_argument_if_absent(argument());
    //SEND(mgenc->arguments, addStringIfAbsent, argument());
	//argument();
}


void Parser::keywordPattern(method_generation_context* mgenc) {
    pString kw;// = String_new("");
    do {
        kw.append(keyword());// kw = SEND(kw, concat, keyword());
		mgenc->add_argument_if_absent(argument());
        //SEND(mgenc->arguments, addStringIfAbsent, argument());
		//argument();
    } while(sym == Keyword);
    
    mgenc->set_signature(kw);// Universe_symbol_for(kw);
    //SEND(kw, free);
}


void Parser::methodBlock(method_generation_context* mgenc) {
    expect(NewTerm);
    blockContents(mgenc);
    // if no return has been generated so far, we can be sure there was no .
    // terminating the last expression, so the last expression's value must be
    // popped off the stack and a ^self be generated
    if(!mgenc->is_finished()) {
        //bytecode_generator::emit_POP(mgenc);
        //bytecode_generator::emit_PUSH_ARGUMENT(mgenc, 0, 0);
        //bytecode_generator::emit_RETURN_LOCAL(mgenc);
        mgenc->set_finished();
    }
    
    expect(EndTerm);
}


pVMSymbol Parser::unarySelector(void) {
	pVMSymbol sym = "symbol "+identifier();
    return sym;//Universe_symbol_for(identifier());
}


pVMSymbol Parser::binarySelector(void) {
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
    
    pVMSymbol symb = "symbol";// Universe_symbol_for(s);
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


void Parser::blockContents(method_generation_context* mgenc) {
    if(accept(Or)) {
        locals(mgenc);
        expect(Or);
    }
    blockBody(mgenc, false);
}


void Parser::locals(method_generation_context* mgenc) {
    while(sym == Identifier) //variable();
		mgenc->add_local_if_absent(variable());
        //SEND(mgenc->locals, addStringIfAbsent, variable());
}


void Parser::blockBody(method_generation_context* mgenc, bool seen_period) {
    if(accept(Exit))
        result(mgenc);
    else if(sym == EndBlock) {
		if(seen_period) {
            // a POP has been generated which must be elided (blocks always
            // return the value of the last expression, regardless of whether it
            // was terminated with a . or not)
			mgenc->dec_bp();
//            mgenc->bp--;
//			mgenc->tp--;
		}
        //bytecode_generator::emit_RETURN_LOCAL(mgenc);
		cout << "emit return local" << endl;
//        mgenc->finished = true;
		mgenc->set_finished();
    } else if(sym == EndTerm) {
        if(seen_period) cout << "emit push argument" << endl;
            // we can be sure it's a method we're parsing, not a block, so the
            // PUSH_ARGUMENT can be created immediately
            //bytecode_generator::emit_PUSH_ARGUMENT(mgenc, 0, 0);
		  cout << "emit return local" << endl;
		  mgenc->set_finished();
		  //bytecode_generator::emit_RETURN_LOCAL(mgenc);
//        mgenc->finished = true;
    } else {
        expression(mgenc);
        if(accept(Period)) {
            //bytecode_generator::emit_POP(mgenc);
			cout << "emit pop" << endl;
            blockBody(mgenc, true);
        }
    }
}


void Parser::result(method_generation_context* mgenc) {
    expression(mgenc);
	cout << "emit return" << endl;
	if(mgenc->is_block_method());
        //bytecode_generator::emit_RETURN_NON_LOCAL(mgenc);
	else ;
        //bytecode_generator::emit_RETURN_LOCAL(mgenc);
    //mgenc->finished = true;
    mgenc->set_finished(true);
	accept(Period);
}


void Parser::expression(method_generation_context* mgenc) {
    PEEK;//peek();
    if(nextSym == Assign)
        assignation(mgenc);
    else
        evaluation(mgenc);
}


void Parser::assignation(method_generation_context* mgenc) {
	list<pString> l;
    
    assignments(mgenc, l);
    evaluation(mgenc);
    list<pString>::iterator i;
    for(i=l.begin(); i != l.end(); ++i)// cout << "emit dup";//SEND(l, size); i++)
        //bytecode_generator::emit_DUP(mgenc);
    for(i=l.begin(); i != l.end(); ++i)//SEND(l, size); i++)
        gen_pop_variable(mgenc, (*i));//SEND(l, get, i));
    
    //SEND(l, deep_free);
}


void Parser::assignments(method_generation_context* mgenc, list<pString> l) {
    if(sym == Identifier) {
        l.push_back(assignment(mgenc));//SEND(l, add, assignment(mgenc));
        PEEK;//peek();
        if(nextSym == Assign)
            assignments(mgenc, l);
    }
}


pString Parser::assignment(method_generation_context* mgenc) {
    pString v = variable();
    pVMSymbol var = "symbol "+v;//Universe_symbol_for(v);
	mgenc->add_literal_if_absent(var);
    //SEND(mgenc->literals, addIfAbsent, var);
    
    expect(Assign);
    
    return v;
}


void Parser::evaluation(method_generation_context* mgenc) {
    bool super;
    primary(mgenc, &super);
    if(sym == Identifier || sym == Keyword || sym == OperatorSequence ||
        symIn(binaryOpSyms)
    ) {       
        messages(mgenc, super);
    }
}


void Parser::primary(method_generation_context* mgenc, bool* super) {
    *super = false;
    switch(sym) {
        case Identifier: {
            pString v = variable();
			if(v == "super") { //SEND(v, compareToChars, "super") == 0) {
                *super = true;
                // sends to super push self as the receiver
                //SEND(v, free);
                v = "self";//String_new("self");
            }
            
            gen_push_variable(mgenc, v);
            //SEND(v, free);
            break;
        }
        case NewTerm:
            nestedTerm(mgenc);
            break;
        case NewBlock: {
            method_generation_context* bgenc = new method_generation_context();
			bgenc->set_is_block_method(true);
			bgenc->set_holder(mgenc->get_holder());
			bgenc->set_outer(mgenc);
            /*method_genc_init(&bgenc);
            bgenc.block_method = true;
            bgenc.holder_genc = mgenc->holder_genc;
            bgenc.outer_genc = mgenc;*/
            
            nestedBlock(bgenc);
            
            //pVMMethod block_method = VMMethod_assemble(&bgenc);
			mgenc->add_literal("block_method");
            //SEND(mgenc->literals, add, block_method)
            //bytecode_generator::emit_PUSH_BLOCK(mgenc, block_method);
			cout << "emit push block" << endl;
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


void Parser::messages(method_generation_context* mgenc, bool super) {
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


void Parser::unaryMessage(method_generation_context* mgenc, bool super) {
    pVMSymbol msg = unarySelector();
	mgenc->add_literal_if_absent(msg);
    //SEND(mgenc->literals, addIfAbsent, msg);
    if(super);
        //bytecode_generator::emit_SUPER_SEND(mgenc, msg);
    else;
        //bytecode_generator::emit_SEND(mgenc, msg);
	cout << endl;
}


void Parser::binaryMessage(method_generation_context* mgenc, bool super) {
    pVMSymbol msg = binarySelector();
	mgenc->add_literal_if_absent(msg);
    //SEND(mgenc->literals, addIfAbsent, msg);
    
    bool tmp_bool = false;
    binaryOperand(mgenc, &tmp_bool);
    
    if(super);
        //bytecode_generator::emit_SUPER_SEND(mgenc, msg);
    else ;
        //bytecode_generator::emit_SEND(mgenc, msg);
	cout << endl;
}


void Parser::binaryOperand(method_generation_context* mgenc, bool* super) {
    primary(mgenc, super);
    
    while(sym == Identifier)
        unaryMessage(mgenc, *super);
}


void Parser::keywordMessage(method_generation_context* mgenc, bool super) {
    pString kw;
    do {
        kw.append(keyword());// = SEND(kw, concat, keyword());
        formula(mgenc);
    } while(sym == Keyword);
    
    pVMSymbol msg = "symbol "+kw;//Universe_symbol_for(kw);
    //SEND(kw, free);
	mgenc->add_literal_if_absent(msg);
    //SEND(mgenc->literals, addIfAbsent, msg);
    
    if(super) ;
        //bytecode_generator::emit_SUPER_SEND(mgenc, msg);
    else ;
        //bytecode_generator::emit_SEND(mgenc, msg);
	cout << endl;
}


void Parser::formula(method_generation_context* mgenc) {
    bool super;
    binaryOperand(mgenc, &super);
    
    // only the first message in a sequence can be a super send
    if(sym == OperatorSequence || symIn(binaryOpSyms))
        binaryMessage(mgenc, super);
    while(sym == OperatorSequence || symIn(binaryOpSyms))
        binaryMessage(mgenc, false);
}


void Parser::nestedTerm(method_generation_context* mgenc) {
    expect(NewTerm);
    expression(mgenc);
    expect(EndTerm);
}


void Parser::literal(method_generation_context* mgenc) {
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


void Parser::literalNumber(method_generation_context* mgenc) {
    int32_t val;
    if(sym == Minus)
        val = negativeDecimal();
    else
        val = literalDecimal();
    
    pVMInteger literal = val;//Universe_new_integer(val);
	stringstream s;
	pString str_lit = "";
	s << literal;
	str_lit += s.str();
	mgenc->add_literal_if_absent(str_lit);
    //SEND(mgenc->literals, addIfAbsent, literal);
    //bytecode_generator::emit_PUSH_CONSTANT(mgenc, (pVMObject)literal);
	cout << "emit push constant literal number" << endl;
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


void Parser::literalSymbol(method_generation_context* mgenc) {
    pVMSymbol symb;
    expect(Pound);
    if(sym == STString) {
        pString s = string();
        symb = "symbol "+s;// Universe_symbol_for(s);
        //SEND(s, free);
    } else
        symb = selector();
	mgenc->add_literal_if_absent(symb);
    //SEND(mgenc->literals, addIfAbsent, symb);
    cout << "emit push constant literal symbol" << endl;
    //bytecode_generator::emit_PUSH_CONSTANT(mgenc, (pVMObject)symb);
}


void Parser::literalString(method_generation_context* mgenc) {
    pString s = string();
	mgenc->add_literal_if_absent(s);
    //pVMString str = s;//Universe_new_string(s);
    //SEND(s, free);
    //
    //SEND(mgenc->literals, addIfAbsent, str);
    //bytecode_generator::emit_PUSH_CONSTANT(mgenc, (pVMObject)str);
    cout << "emit push constant literal string" << endl;
}


pVMSymbol Parser::selector(void) {
    if(sym == OperatorSequence || symIn(singleOpSyms))
        return binarySelector();
    else if(sym == Keyword || sym == KeywordSequence)
        return keywordSelector();
    else
        return unarySelector();
}


pVMSymbol Parser::keywordSelector(void) {
    pString s(text);// = String_new(text);
    expectOneOf(keywordSelectorSyms);
    pVMSymbol symb = "symbol "+s;//Universe_symbol_for(s);
    //SEND(s, free);
    return symb;
}


pString Parser::string(void) {
    pString s(text); 
    expect(STString);    
    return s; // <-- Literal strings are At Most BUFSIZ chars long.
}


void Parser::nestedBlock(method_generation_context* mgenc) {
    #define BLOCK_METHOD_S "$block method"
    #define BLOCK_METHOD_LEN (13)
	mgenc->add_argument_if_absent("$block self");
    //SEND(mgenc->arguments, addStringIfAbsent, String_new("$block self"));
    
    expect(NewBlock);
    if(sym == Colon)
        blockPattern(mgenc);
    
    // generate Block signature
    pString block_sig = BLOCK_METHOD_S;// String_new(BLOCK_METHOD_S);
	size_t arg_size = mgenc->get_number_of_arguments();// SEND(mgenc->arguments, size);
    for(size_t i = 1; i < arg_size; i++)
		block_sig += ":";//.append(':');
        //SEND(block_sig, concatChar, ':');

    mgenc->set_signature(block_sig); // Universe_symbol_for(block_sig);
    
    blockContents(mgenc);
    
    // if no return has been generated, we can be sure that the last expression
    // in the block was not terminated by ., and can generate a return
    if(!mgenc->is_finished()) {
		//bytecode_generator::emit_RETURN_LOCAL(mgenc);
        mgenc->set_finished(true);
    }
    
    expect(EndBlock);
}


void Parser::blockPattern(method_generation_context* mgenc) {
    blockArguments(mgenc);
    expect(Or);
}


void Parser::blockArguments(method_generation_context* mgenc) {
    do {
        expect(Colon);
		mgenc->add_argument_if_absent(argument());
        //SEND(mgenc->arguments, addStringIfAbsent, argument());
		argument();
    } while(sym == Colon);
}


