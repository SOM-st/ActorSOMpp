#include "bytecodes.h"

const uint8_t Bytecode::bytecodeLengths[] = {
    1, // BC_HALT
    1, // BC_DUP
    3, // BC_PUSH_LOCAL
    3, // BC_PUSH_ARGUMENT
    2, // BC_PUSH_FIELD
    2, // BC_PUSH_BLOCK
    2, // BC_PUSH_CONSTANT
    2, // BC_PUSH_GLOBAL
    1, // BC_POP
    3, // BC_POP_LOCAL
    3, // BC_POP_ARGUMENT
    2, // BC_POP_FIELD
    2, // BC_SEND
    2, // BC_SUPER_SEND
    1, // BC_RETURN_LOCAL
    1  // BC_RETURN_NON_LOCAL
};

const char* Bytecode::bytecodeNames[] = {
    "HALT            ",
    "DUP             ",
    "PUSH_LOCAL      ",
    "PUSH_ARGUMENT   ",
    "PUSH_FIELD      ",
    "PUSH_BLOCK      ",
    "PUSH_CONSTANT   ",
    "PUSH_GLOBAL     ",
    "POP             ",
    "POP_LOCAL       ",
    "POP_ARGUMENT    ",
    "POP_FIELD       ",
    "SEND            ",
    "SUPER_SEND      ",
    "RETURN_LOCAL    ",
    "RETURN_NON_LOCAL"
};


