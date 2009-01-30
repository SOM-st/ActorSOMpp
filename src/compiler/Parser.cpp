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
#include "../vmobjects/VMPrimitive.h"
#include "../vmobjects/VMObject.h"

#include "BytecodeGenerator.h"
#include "../vmobjects/VMSymbol.h"
#include "../vm/Universe.h"

#include <iostream>
#include <cctype>
#include <sstream>
#include <stdlib.h>
#include <string.h>
//#include <memory/gc.h>
//#include <vmobjects/VMString.h>


#define GETSYM sym = lexer->GetSym(); \
			   text = lexer->GetText()

#define PEEK nextSym = lexer->Peek(); \
			 nextText = lexer->GetNextText()

Parser::Parser(istream& file) {
	//lexer(file);
    sym = NONE;
    lexer = new Lexer(file);
    bcGen = new BytecodeGenerator();
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

void Parser::assignments( MethodGenerationContext* mgenc, list<StdString> l )
{
    // TODO
}

*/

Parser::~Parser() {
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
	fprintf(stderr, ": %s\n", lexer->GetRawBuffer().c_str());
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
	fprintf(stderr, ": %s\n", lexer->GetRawBuffer().c_str());
    return false;
}




void Parser::genPushVariable(MethodGenerationContext* mgenc, const StdString& var) {
    // The purpose of this function is to find out whether the variable to be
    // pushed on the stack is a local variable, argument, or object field. This
    // is done by examining all available lexical contexts, starting with the
    // innermost (i.e., the one represented by mgenc).
    int index = 0;
    int context = 0;
    bool is_argument = false;
    
    if(mgenc->FindVar(var, &index, &context, &is_argument)) {
		if(is_argument) 
            bcGen->emit_PUSH_ARGUMENT(mgenc, index, context);
        else 
            bcGen->emit_PUSH_LOCAL(mgenc, index, context);
    } else if(mgenc->FindField(var)) {
        pVMSymbol fieldName = _UNIVERSE->SymbolFor(var);
		mgenc->AddLiteralIfAbsent((pVMObject)fieldName);
        bcGen->emit_PUSH_FIELD(mgenc, fieldName);
		//SEND(mgenc->literals, addIfAbsent, fieldName);
        //BytecodeGenerator::emit_PUSH_FIELD(mgenc, fieldName);
    } else {
        
        pVMSymbol global = _UNIVERSE->SymbolFor(var);
		mgenc->AddLiteralIfAbsent((pVMObject)global);
        //SEND(mgenc->literals, addIfAbsent, global);
        bcGen->emit_PUSH_GLOBAL(mgenc, global);
    }
}


void Parser::genPopVariable(MethodGenerationContext* mgenc, const StdString& var){
    // The purpose of this function is to find out whether the variable to be
    // popped off the stack is a local variable, argument, or object field. This
    // is done by examining all available lexical contexts, starting with the
    // innermost (i.e., the one represented by mgenc).
    int index = 0;
    int context = 0;
    bool is_argument = false;
	//cout << "emit pop arg/local/field" << endl;
    if(mgenc->FindVar(var, &index, &context, &is_argument)) {
        if(is_argument) bcGen->emit_POP_ARGUMENT(mgenc, index, context);
        else bcGen->emit_POP_LOCAL(mgenc, index, context);
    } else bcGen->emit_POP_FIELD(mgenc, _UNIVERSE->SymbolFor(var));
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




void Parser::Classdef(ClassGenerationContext* cgenc) {
//    cgenc->name = Universe_symbol_for_chars(text);
	cgenc->SetName(_UNIVERSE->SymbolFor(text));
    expect(Identifier);
    
    expect(Equal);
    
    if(sym == Identifier) {
       // cgenc->super_name = Universe_symbol_for_chars(text);
		cgenc->SetSuperName(_UNIVERSE->SymbolFor(text));
        accept(Identifier);
    } else cgenc->SetSuperName(_UNIVERSE->SymbolFor("Object"));
       // cgenc->super_name = Universe_symbol_for_chars("Object");
    
    expect(NewTerm);
    instanceFields(cgenc);
    while(sym == Identifier || sym == Keyword || sym == OperatorSequence ||
        symIn(binaryOpSyms)) {
		
        MethodGenerationContext* mgenc = new MethodGenerationContext();
		mgenc->SetHolder(cgenc);
		mgenc->AddArgument("self");
    
        method(mgenc);
        
		if(mgenc->IsPrimitive())
            cgenc->AddInstanceMethod(dynamic_cast<pVMObject>(mgenc->AssemblePrimitive()));
            //SEND(cgenc->instance_methods, add, VMPrimitive_assemble(&mgenc));
        else
			cgenc->AddInstanceMethod(dynamic_cast<pVMObject>(mgenc->Assemble()));
            //SEND(cgenc->instance_methods, add, VMMethod_assemble(&mgenc));
        delete(mgenc);
        //method_genc_release(&mgenc);
    }
    
    if(accept(Separator)) {
        cgenc->SetClassSide(true);
        classFields(cgenc);
        while(sym == Identifier || sym == Keyword || sym == OperatorSequence ||
                symIn(binaryOpSyms)) {
            MethodGenerationContext* mgenc = new MethodGenerationContext();
			mgenc->SetHolder(cgenc);
			mgenc->AddArgument("self");
            //method_genc_init(&mgenc);
            //mgenc.holder_genc = cgenc;
            //
            //// each method has a self variable
            //SEND(mgenc.arguments, add, String_new("self"));
            //
            method(mgenc);
            
			if(mgenc->IsPrimitive())
                cgenc->AddClassMethod((pVMObject)mgenc->AssemblePrimitive());
                //SEND(cgenc->class_methods, add, VMPrimitive_assemble(&mgenc));
            else
				cgenc->AddClassMethod((pVMObject)mgenc->Assemble());
                //SEND(cgenc->class_methods, add, VMMethod_assemble(&mgenc));
            delete(mgenc);
            //method_genc_release(&mgenc);
        }    
    }
    expect(EndTerm);
}


void Parser::instanceFields(ClassGenerationContext* cgenc) {
    if(accept(Or)) {
        while(sym == Identifier) {
            StdString var = variable();
            cgenc->AddInstanceField((pVMObject)_UNIVERSE->SymbolFor(var));
            //SEND(cgenc->instance_fields, add, Universe_symbol_for(var));
            //SEND(var, free);
        }
        expect(Or);
    }
}


void Parser::classFields(ClassGenerationContext* cgenc) {
    if(accept(Or)) {
        while(sym == Identifier) {
            StdString var = variable();
			cgenc->AddClassField((pVMObject)_UNIVERSE->SymbolFor(var));
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
		mgenc->SetPrimitive(true);
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
    mgenc->SetSignature(unarySelector());
}


void Parser::binaryPattern(MethodGenerationContext* mgenc) {
    mgenc->SetSignature(binarySelector());
	mgenc->AddArgumentIfAbsent(argument());
    //SEND(mgenc->arguments, addStringIfAbsent, argument());
	//argument();
}


void Parser::keywordPattern(MethodGenerationContext* mgenc) {
    StdString kw;// = String_new("");
    do {
        kw.append(keyword());// kw = SEND(kw, concat, keyword());
		mgenc->AddArgumentIfAbsent(argument());
        //SEND(mgenc->arguments, addStringIfAbsent, argument());
		//argument();
    } while(sym == Keyword);
    
    mgenc->SetSignature(_UNIVERSE->SymbolFor(kw));// Universe_symbol_for(kw);
    //SEND(kw, free);
}


void Parser::methodBlock(MethodGenerationContext* mgenc){
    expect(NewTerm);
    blockContents(mgenc);
    // if no return has been generated so far, we can be sure there was no .
    // terminating the last expression, so the last expression's value must be
    // popped off the stack and a ^self be generated
    if(!mgenc->IsFinished()) {
        bcGen->emit_POP(mgenc);
        bcGen->emit_PUSH_ARGUMENT(mgenc, 0, 0);
        bcGen->emit_RETURN_LOCAL(mgenc);
        mgenc->SetFinished();
    }
    
    expect(EndTerm);
}


pVMSymbol Parser::unarySelector(void) {
    return _UNIVERSE->SymbolFor(identifier());
}


pVMSymbol Parser::binarySelector(void) {
    StdString s(text);
    
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
    
    pVMSymbol symb = _UNIVERSE->SymbolFor(s);
    //SEND(s, free);
    return symb;
}


StdString Parser::identifier(void) {
    StdString s(text);
    if(accept(Primitive))
        ; // text is set
    else
        expect(Identifier);
    
    return s;
}


StdString Parser::keyword(void) {
    StdString s(text);
    expect(Keyword);
    
    return s;
}


StdString Parser::argument(void) {
    return variable();
}


void Parser::blockContents(MethodGenerationContext* mgenc) {
    if(accept(Or)) {
        locals(mgenc);
        expect(Or);
    }
    blockBody(mgenc, false);
}


void Parser::locals(MethodGenerationContext* mgenc)
{
    while(sym == Identifier) //variable();
		mgenc->AddLocalIfAbsent(variable());
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
		mgenc->SetFinished();
    } else if(sym == EndTerm) {
        if(seen_period)
            // we can be sure it's a method we're parsing, not a block, so the
            // PUSH_ARGUMENT can be created immediately
            bcGen->emit_PUSH_ARGUMENT(mgenc, 0, 0);
		bcGen->emit_RETURN_LOCAL(mgenc);
		mgenc->SetFinished();
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
	if(mgenc->IsBlockMethod()) bcGen->emit_RETURN_NON_LOCAL(mgenc);
	else bcGen->emit_RETURN_LOCAL(mgenc);
    //mgenc->finished = true;
    mgenc->SetFinished(true);
	accept(Period);
}


void Parser::expression(MethodGenerationContext* mgenc) {
    PEEK;//Peek();
    if(nextSym == Assign)
        assignation(mgenc);
    else
        evaluation(mgenc);
}


void Parser::assignation(MethodGenerationContext* mgenc) {
	list<StdString> l;
    //cout << "assignation" << endl;
    assignments(mgenc, l);
    evaluation(mgenc);
    list<StdString>::iterator i;
    for(i=l.begin(); i != l.end(); ++i)
        bcGen->emit_DUP(mgenc);
    for(i=l.begin(); i != l.end(); ++i)//SEND(l, size); i++)
        genPopVariable(mgenc, (*i));//SEND(l, get, i));
    
    //SEND(l, deep_free);
}


void Parser::assignments(MethodGenerationContext* mgenc, list<StdString>& l) {
    if(sym == Identifier) {
        l.push_back(assignment(mgenc));//SEND(l, add, assignment(mgenc));
        PEEK;//Peek();
        if(nextSym == Assign)
            assignments(mgenc, l);
    }
}


StdString Parser::assignment(MethodGenerationContext* mgenc) {
    StdString v = variable();
    pVMSymbol var = _UNIVERSE->SymbolFor(v);
	mgenc->AddLiteralIfAbsent((pVMObject)var);
    //SEND(mgenc->literals, addIfAbsent, var);
    
    expect(Assign);
    
    return v;
}


void Parser::evaluation(MethodGenerationContext* mgenc) {
    bool super;
    primary(mgenc, &super);
    if(sym == Identifier || sym == Keyword || sym == OperatorSequence ||
        symIn(binaryOpSyms)) {       
        messages(mgenc, super);
    }
}


void Parser::primary(MethodGenerationContext* mgenc, bool* super) {
    *super = false;
    switch(sym) {
        case Identifier: {
            StdString v = variable();
			if(v == "super") { 
                *super = true;
                // sends to super push self as the receiver
                //SEND(v, free);
                v = StdString("self");//String_new("self");
            }
            
            genPushVariable(mgenc, v);
            //SEND(v, free);
            break;
        }
        case NewTerm:
            nestedTerm(mgenc);
            break;
        case NewBlock: {
            MethodGenerationContext* bgenc = new MethodGenerationContext();
			bgenc->SetIsBlockMethod(true);
			bgenc->SetHolder(mgenc->GetHolder());
			bgenc->SetOuter(mgenc);
            /*method_genc_init(&bgenc);
            bgenc.block_method = true;
            bgenc.holder_genc = mgenc->holder_genc;
            bgenc.outer_genc = mgenc;*/
            
            nestedBlock(bgenc);
            
            pVMMethod block_method = bgenc->Assemble();
			mgenc->AddLiteral(block_method);
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


StdString Parser::variable(void) {
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
    pVMSymbol msg = unarySelector();
	mgenc->AddLiteralIfAbsent((pVMObject)msg);
    //SEND(mgenc->literals, addIfAbsent, msg);
    if(super) bcGen->emit_SUPER_SEND(mgenc, msg);
    else bcGen->emit_SEND(mgenc, msg);
	//cout << endl;
}


void Parser::binaryMessage(MethodGenerationContext* mgenc, bool super) {
    pVMSymbol msg = binarySelector();
	mgenc->AddLiteralIfAbsent((pVMObject)msg);
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
    StdString kw;
    do {
        kw.append(keyword());// = SEND(kw, concat, keyword());
        formula(mgenc);
    } while(sym == Keyword);
    
    pVMSymbol msg = _UNIVERSE->SymbolFor(kw);
    //SEND(kw, free);
	mgenc->AddLiteralIfAbsent((pVMObject)msg);
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
    
    pVMInteger lit = _UNIVERSE->NewInteger(val);
	/*stringstream s;
	StdString str_lit = "";
	s << literal->GetEmbeddedInteger();
	str_lit += s.str();*/
	mgenc->AddLiteralIfAbsent((pVMObject)lit);
    //SEND(mgenc->literals, addIfAbsent, literal);
    bcGen->emit_PUSH_CONSTANT(mgenc, (pVMObject)lit);
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
    pVMSymbol symb;
    expect(Pound);
    if(sym == STString) {
        StdString s = _string();
        symb = _UNIVERSE->SymbolFor(s);
        //SEND(s, free);
    } else
        symb = selector();
	mgenc->AddLiteralIfAbsent((pVMObject)symb);
    //SEND(mgenc->literals, addIfAbsent, symb);
    //cout << "emit push constant literal symbol" << endl;
    bcGen->emit_PUSH_CONSTANT(mgenc, (pVMObject)symb);
}


void Parser::literalString(MethodGenerationContext* mgenc) {
    StdString s = _string();
	
    pVMString str = _UNIVERSE->NewString(s);
    mgenc->AddLiteralIfAbsent((pVMObject)str);
    //SEND(s, free);
    //
    //SEND(mgenc->literals, addIfAbsent, str);
    bcGen->emit_PUSH_CONSTANT(mgenc,(pVMObject)str);
    //cout << "emit push constant literal string" << endl;
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
    StdString s(text);// = String_new(text);
    expectOneOf(keywordSelectorSyms);
    pVMSymbol symb = _UNIVERSE->SymbolFor(s);
    //SEND(s, free);
    return symb;
}


StdString Parser::_string(void) {
    StdString s(text); 
    expect(STString);    
    return s; // <-- Literal strings are At Most BUFSIZ chars long.
}


void Parser::nestedBlock(MethodGenerationContext* mgenc) {
    #define BLOCK_METHOD_S "$block method"
    #define BLOCK_METHOD_LEN (13)
	mgenc->AddArgumentIfAbsent("$block self");
    //SEND(mgenc->arguments, addStringIfAbsent, String_new("$block self"));
    
    expect(NewBlock);
    if(sym == Colon)
        blockPattern(mgenc);
    
    // generate Block signature
    StdString block_sig = StdString(BLOCK_METHOD_S);// String_new(BLOCK_METHOD_S);
	size_t arg_size = mgenc->GetNumberOfArguments();// SEND(mgenc->arguments, size);
    for(size_t i = 1; i < arg_size; i++)
		block_sig += ":";//.append(':');
        //SEND(block_sig, concatChar, ':');

    mgenc->SetSignature(_UNIVERSE->SymbolFor(block_sig));
    
    blockContents(mgenc);
    
    // if no return has been generated, we can be sure that the last expression
    // in the block was not terminated by ., and can generate a return
    if(!mgenc->IsFinished()) {
		bcGen->emit_RETURN_LOCAL(mgenc);
        mgenc->SetFinished(true);
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
		mgenc->AddArgumentIfAbsent(argument());
        //SEND(mgenc->arguments, addStringIfAbsent, argument());
		//argument();
    } while(sym == Colon);
}


