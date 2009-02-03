#include "VMBlock.h"
#include "VMMethod.h"
#include "VMFrame.h"
#include "VMEvaluationPrimitive.h"
#include "../vm/Universe.h"

const int VMBlock::VMBlockNumberOfFields = 2; 

VMBlock::VMBlock() : VMObject(VMBlockNumberOfFields) {
    //this->SetNumberOfFields(this->GetNumberOfFields() + 2);
     blockMethod = Globals::NilObject();
     context = (pVMFrame) Globals::NilObject();
}

void VMBlock::SetMethod(pVMMethod bMethod) {
    blockMethod = dynamic_cast<pVMObject>(bMethod);
}


pVMMethod VMBlock::GetMethod() const {
    return dynamic_cast<pVMMethod>(blockMethod);
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
