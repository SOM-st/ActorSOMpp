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
#include <fstream>
#include <string>
#include "../misc/defs.h"

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

class Lexer {

public:
	Lexer(ifstream& file);
	~Lexer();
	Symbol getsym(void);
	Symbol peek(void);
	//todo: change peek/getsym to take a string* for the text/nextText
	pString gettext(void);
	pString getnexttext(void);
	pString getrawbuffer(void);

private:
	void fillbuffer(void);
	void skipWhiteSpace(void);
	void skipComment(void);	
	

	ifstream& infile;

	Symbol sym;
	char symc;
	char text[BUFSIZ];

	bool peekDone;
	Symbol nextSym;
	char nextSymc;
	char nextText[BUFSIZ];
	
	std::string buf;
	unsigned int bufp;
};