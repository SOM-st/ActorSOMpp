#pragma once
#ifndef BYTECODES_H_
#define BYTECODES_H_

/*
 *
 *
Copyright (c) 2007 Michael Haupt, Tobias Pape, Arne Bergmann
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

 
#include "../misc/defs.h"

// bytecode definitions
//      tmpName,            Name, ConstantValue, Length, StringName
#define bc_HALT             HALT,              0,  1, "HALT            "
#define bc_DUP              DUP,               1,  1, "DUP             "
#define bc_PUSH_LOCAL       PUSH_LOCAL,        2,  3, "PUSH_LOCAL      "
#define bc_PUSH_ARGUMENT    PUSH_ARGUMENT,     3,  3, "PUSH_ARGUMENT   "
#define bc_PUSH_FIELD       PUSH_FIELD,        4,  2, "PUSH_FIELD      "
#define bc_PUSH_BLOCK       PUSH_BLOCK,        5,  2, "PUSH_BLOCK      "
#define bc_PUSH_CONSTANT    PUSH_CONSTANT,     6,  2, "PUSH_CONSTANT   "
#define bc_PUSH_GLOBAL      PUSH_GLOBAL,       7,  2, "PUSH_GLOBAL     "
#define bc_POP              POP,               8,  1, "POP             "
#define bc_POP_LOCAL        POP_LOCAL,         9,  3, "POP_LOCAL       "
#define bc_POP_ARGUMENT     POP_ARGUMENT,     10,  3, "POP_ARGUMENT    "
#define bc_POP_FIELD        POP_FIELD,        11,  2, "POP_FIELD       "
#define bc_SEND             SEND,             12,  2, "SEND            "
#define bc_SUPER_SEND       SUPER_SEND,       13,  2, "SUPER_SEND      "
#define bc_RETURN_LOCAL     RETURN_LOCAL,     14,  1, "RETURN_LOCAL    "
#define bc_RETURN_NON_LOCAL RETURN_NON_LOCAL, 15,  1, "RETURN_NON_LOCAL"

// the new bytecode definition uses the concept of first-class templates
// learned from David, and exaggerated to the max :D
// But, I think it will make my life a bit easier by keeping bytecode definition
// easy and at one place
// CALL_TEMPLATE is needed to force expansion of tempalte name and arguments
// at the same time
#define CALL_TEMPLATE(template, params) template(params) 
#define SOM_BYTECODES(template) \
    CALL_TEMPLATE(template, bc_HALT            ) \
    CALL_TEMPLATE(template, bc_DUP             ) \
    CALL_TEMPLATE(template, bc_PUSH_LOCAL      ) \
    CALL_TEMPLATE(template, bc_PUSH_ARGUMENT   ) \
    CALL_TEMPLATE(template, bc_PUSH_FIELD      ) \
    CALL_TEMPLATE(template, bc_PUSH_BLOCK      ) \
    CALL_TEMPLATE(template, bc_PUSH_CONSTANT   ) \
    CALL_TEMPLATE(template, bc_PUSH_GLOBAL     ) \
    CALL_TEMPLATE(template, bc_POP             ) \
    CALL_TEMPLATE(template, bc_POP_LOCAL       ) \
    CALL_TEMPLATE(template, bc_POP_ARGUMENT    ) \
    CALL_TEMPLATE(template, bc_POP_FIELD       ) \
    CALL_TEMPLATE(template, bc_SEND            ) \
    CALL_TEMPLATE(template, bc_SUPER_SEND      ) \
    CALL_TEMPLATE(template, bc_RETURN_LOCAL    ) \
    CALL_TEMPLATE(template, bc_RETURN_NON_LOCAL)

// add bytecodes for actor VM
#define bc_SEND_ASYNC       SEND_ASYNC,        16,  1, "SEND_ASYNC      "
#define bc_YIELD            YIELD,             17,  1, "YIELD           "

// define set of actor bytecodes
#define ACTOR_BYTECODES(template) \
    CALL_TEMPLATE(template, bc_SEND_ASYNC     ) \
    CALL_TEMPLATE(template, bc_YIELD          )


// define set of all bytecodes to be used
#define FOR_ALL_BYTECODES(template) \
    SOM_BYTECODES(template) \
    ACTOR_BYTECODES(template)

// bytecode constants used by SOM++
#define define_CONSTANTS(MacroName, ConstantValue, Length, StringName) \
    BC_##MacroName = ConstantValue,

enum Bytecodes {
    FOR_ALL_BYTECODES(define_CONSTANTS)
    end_enum // just to overcome a superflous comma
};

class Bytecode {

public:
    static char* GetBytecodeName(uint8_t bc) {
        return (char*)bytecodeNames[bc];
    }

    static uint8_t GetBytecodeLength(uint8_t bc) {

        return bytecodeLengths[bc];// Return the length of the given bytecode
    }

private:
    
static const uint8_t bytecodeLengths[];

static const char* bytecodeNames[];
};



#endif
