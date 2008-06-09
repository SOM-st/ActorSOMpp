#pragma once

#include "../misc/defs.h"
#include "ClassGenerationContext.h"
#include <vector>

class method_generation_context {
	class_generation_context*  holder_genc;
    method_generation_context* outer_genc;
    bool                       block_method;
    pVMSymbol                  signature;
    pList                      arguments;
    bool                       primitive;
    pList                      locals;
    pList                      literals;
    bool                       finished;
    uint32_t                   bp;
	vector<uint8_t>            bytecode;
};