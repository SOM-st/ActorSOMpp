#include "VMBlock.h"
#include "VMMethod.h"
#include "VMFrame.h"
#include "VMEvaluationPrimitive.h"
#include "../vm/Universe.h"


VMBlock::VMBlock() : VMObject(2) {
    //this->SetNumberOfFields(this->GetNumberOfFields() + 2);
     blockMethod = (pVMMethod) Globals::NilObject();
     context = (pVMFrame) Globals::NilObject();
}


//VMBlock::~VMBlock()
//{
//}


void VMBlock::MarkReferences() {
    if (gcfield) return;
    VMObject::MarkReferences();
    /*if (blockMethod != NULL) blockMethod->MarkReferences();
    if (context != NULL) context->MarkReferences();*/
    
}



pVMEvaluationPrimitive VMBlock::GetEvaluationPrimitive(int numberOfArguments) {
    return new (_HEAP) VMEvaluationPrimitive(numberOfArguments);
}
