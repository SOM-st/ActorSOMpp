#pragma once

#include "../misc/defs.h"
#include "ClassGenerationContext.h"
#include <vector>

class MethodGenerationContext {
	ClassGenerationContext*  holder_genc;
    MethodGenerationContext* outer_genc;
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