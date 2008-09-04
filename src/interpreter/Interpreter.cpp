#include "Interpreter.h"
#include "../vmobjects/VMMethod.h"
#include "../vmobjects/VMFrame.h"
#include "../vmobjects/VMMethod.h"
#include "../vmobjects/VMClass.h"
#include "../vmobjects/VMObject.h"
#include "../vmobjects/VMSymbol.h"

Interpreter::Interpreter()
{
    // TODO
}

Interpreter::~Interpreter()
{
    // TODO
}

void Interpreter::Start()
{
    // TODO
}

VMFrame* Interpreter::PushNewFrame( VMMethod* method )
{
    return NULL;
}

void Interpreter::SetFrame( VMFrame* frame )
{
    // TODO
}

VMFrame* Interpreter::GetFrame()
{
    return NULL;
}

VMMethod* Interpreter::GetMethod()
{
    return NULL;
}

VMObject* Interpreter::GetSelf()
{
    return NULL;
}

VMFrame* Interpreter::popFrame()
{
    return NULL;
}

void Interpreter::popFrameAndPushResult( VMObject* result )
{
    // TODO
}

void Interpreter::send( VMSymbol* signature, VMClass* receiver_class )
{
    // TODO
}

void Interpreter::do_dup()
{
    // TODO
}

void Interpreter::do_push_local( int bytecode_index )
{
    // TODO
}

void Interpreter::do_push_argument( int bytecode_index )
{
    // TODO
}

void Interpreter::do_push_field( int bytecode_index )
{
    // TODO
}

void Interpreter::do_push_block( int bytecode_index )
{
    // TODO
}

void Interpreter::do_push_constant( int bytecode_index )
{
    // TODO
}

void Interpreter::do_push_global( int bytecode_index )
{
    // TODO
}

void Interpreter::do_pop()
{
    // TODO
}

void Interpreter::do_pop_local( int bytecode_index )
{
    // TODO
}

void Interpreter::do_pop_argument( int bytecode_index )
{
    // TODO
}

void Interpreter::do_pop_field( int bytecode_index )
{
    // TODO
}

void Interpreter::do_send( int bytecode_index )
{
    // TODO
}

void Interpreter::do_super_send( int bytecode_index )
{
    // TODO
}

void Interpreter::do_return_local()
{
    // TODO
}

void Interpreter::do_return_non_local()
{
    // TODO
}

