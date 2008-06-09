#include "Lexer.h"


Lexer::Lexer(ifstream &file) : infile(file) {
	peekDone = false;
	bufp = 0;
}

Lexer::~Lexer() {
}


pString Lexer::gettext(void) {
	return pString(text);
}

pString Lexer::getnexttext(void) {
	return pString(nextText);
}

pString Lexer::getrawbuffer(void) {
	//for debug
	return pString(buf);
}

#define _BC (buf[bufp])
#define EOB (bufp >= buf.length())

void Lexer::fillbuffer(void) {
	if(!infile.good()) // string stream
        return;

	std::getline(infile, buf);
    bufp = 0;
}


//
// basic lexing
//
#pragma mark Lexer functions

void Lexer::skipWhiteSpace(void) {
    while(isspace(_BC)) {
        bufp++;
        if(EOB)
            fillbuffer();
    }
}


void Lexer::skipComment(void) {
	//cout << "skipComment" << endl;
    if(_BC == '"') {
		//cout << "skipComment" << endl;
        do {
            bufp++;
            if(EOB)
                fillbuffer();
			//cout << _BC;
        } while(_BC != '"');
        bufp++;
    }
}


#define _ISOP(C) \
    ((C) == '~' || (C) == '&' || (C) == '|' || (C) == '*' || (C) == '/' || \
     (C) == '\\' || (C) == '+' || (C) == '=' || (C) == '>' || (C) == '<' || \
     (C) == ',' || (C) == '@' || (C) == '%')
#define _MATCH(C, S) \
    if(_BC == (C)) { sym = (S); symc = _BC; sprintf(text, "%c", _BC); bufp++;}
#define SEPARATOR std::string("----") //FIXME
#define PRIMITIVE std::string("primitive")

Symbol Lexer::getsym(void) {
	//cout << "getsym ";
    if(peekDone) {
        peekDone = false;
        sym = nextSym;
        symc = nextSymc;
        strcpy(text, nextText);
        return sym;
    }

    do {
        if(EOB)
            fillbuffer();
        skipWhiteSpace();
        skipComment();
		//cout << "ws ";
    } while((EOB || isspace(_BC) || _BC == '"') && infile.good());
    //cout << endl << _BC << endl;
    if(_BC == '\'') {
        sym = STString;
        symc = 0;
        char* t = text;
        do {
            *t++ = buf[++bufp];
			//cout << _BC;
        } while(_BC != '\'');
		//cout << endl << _BC << endl;
        bufp++;
        *--t = 0;
    }
    else _MATCH('[', NewBlock)
    else _MATCH(']', EndBlock)
    else if(_BC == ':') {
        if(buf[bufp+1] == '=') {
            bufp += 2;
            sym = Assign;
            symc = 0;
            sprintf(text, ":=");
        } else {
            bufp++;
            sym = Colon;
            symc = ':';
            sprintf(text, ":");
        }
    }
    else _MATCH('(', NewTerm)
    else _MATCH(')', EndTerm)
    else _MATCH('#', Pound)
    else _MATCH('^', Exit)
    else _MATCH('.', Period)
    else if(_BC == '-') {
		if(!buf.substr(bufp, SEPARATOR.length()).compare(SEPARATOR)) {// !strncmp(buf + bufp, SEPARATOR, strlen(SEPARATOR))) {
            char* t = text;
            while(_BC == '-')
                *t++ = buf[bufp++];
            *t = 0;
            sym = Separator;
        } else {
            bufp++;
            sym = Minus;
            symc = '-';
            sprintf(text, "-");
        }
    }
    else if(_ISOP(_BC)) {
        if(_ISOP(buf[bufp + 1])) {
            sym = OperatorSequence;
            symc = 0;
            char* t = text;
            while(_ISOP(_BC))
                *t++ = buf[bufp++];
            *t = 0;
        }
        else _MATCH('~', Not)
        else _MATCH('&', And)
        else _MATCH('|', Or)
        else _MATCH('*', Star)
        else _MATCH('/', Div)
        else _MATCH('\\', Mod)
        else _MATCH('+', Plus)
        else _MATCH('=', Equal)
        else _MATCH('>', More)
        else _MATCH('<', Less)
        else _MATCH(',', Comma)
        else _MATCH('@', At)
        else _MATCH('%', Per)
    }
    else if (!buf.substr(bufp, PRIMITIVE.length()).compare(PRIMITIVE)) {
        bufp += PRIMITIVE.length();
        sym = Primitive;
        symc = 0;
        sprintf(text, "primitive");
    }
    else if(isalpha(_BC)) {
        char* t = text;
        symc = 0;
        while(isalpha(_BC) || isdigit(_BC) || _BC == '_')
            *t++ = buf[bufp++];
        sym = Identifier;
        if(buf[bufp] == ':') {
            sym = Keyword;
            bufp++;
            *t++ = ':';
            if(isalpha(_BC)) {
                sym = KeywordSequence;
                while(isalpha(_BC) || _BC == ':')
                    *t++ = buf[bufp++];
            }
        }
        *t = 0;
    }
    else if(isdigit(_BC)) {
        sym = Integer;
        symc = 0;
        char* t = text;
        do {
            *t++ = buf[bufp++];
        } while(isdigit(_BC));
        *t = 0;
    }
    else {
        sym = NONE;
        symc = _BC;
        sprintf(text, "%c", _BC);
    }
	//cout << endl;
	return sym;
}


Symbol Lexer::peek(void) {
    Symbol saveSym = sym;
    char saveSymc = symc;
    char saveText[256];
    strcpy(saveText, text);
    if(peekDone)
        fprintf(stderr, "Cannot peek twice!\n");
    getsym();
    nextSym = sym;
    nextSymc = symc;
    strcpy(nextText, text);
    sym = saveSym;
    symc = saveSymc;
    strcpy(text, saveText);
    peekDone = true;
	return nextSym;
}