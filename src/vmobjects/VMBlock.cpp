#include "VMBlock.h"
#include "VMMethod.h"
#include "VMFrame.h"
#include "VMEvaluationPrimitive.h"
#include "../vm/Universe.h"

VMBlock::VMBlock() : VMObject(2)
{
    //this->SetNumberOfFields(this->GetNumberOfFields() + 2);
     blockMethod = (VMMethod*) nil_object;
     context = (VMFrame*) nil_object;
}

//VMBlock::~VMBlock()
//{
//}

void VMBlock::MarkReferences()
{
    if (gcfield) return;
    VMObject::MarkReferences();
    /*if (blockMethod != NULL) blockMethod->MarkReferences();
    if (context != NULL) context->MarkReferences();*/
    
}

void VMBlock::SetMethod(VMMethod* bMethod)
{
    blockMethod = bMethod;
}

VMMethod* VMBlock::GetMethod()
{
    return blockMethod;
}

void VMBlock::SetContext(VMFrame* contxt)
{
    context = contxt;
}

VMFrame* VMBlock::GetContext()
{
    return context;
}

VMEvaluationPrimitive* VMBlock::GetEvaluationPrimitive(int numberOfArguments)
{
    return new (_HEAP) VMEvaluationPrimitive(numberOfArguments);
}
