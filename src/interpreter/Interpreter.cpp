#include "Interpreter.h"
#include "bytecodes.h"

#include "../vmobjects/VMMethod.h"
#include "../vmobjects/VMFrame.h"
#include "../vmobjects/VMMethod.h"
#include "../vmobjects/VMClass.h"
#include "../vmobjects/VMObject.h"
#include "../vmobjects/VMSymbol.h"
#include "../vmobjects/VMInvokable.h"
#include "../vmobjects/Signature.h"

#include "../compiler/Disassembler.h"


// convenience macros for frequently used function invocations
#define _FRAME this->GetFrame()
#define _SETFRAME(f) this->SetFrame(f)
#define _METHOD this->GetMethod()
#define _SELF this->GetSelf()


Interpreter::Interpreter() {
    this->frame = NULL;
    
    uG = "unknownGlobal:";
    dnu = "doesNotUnderstand:arguments:";
    eB = "escapedBlock:";
    // TODO
}


Interpreter::~Interpreter() {
    // TODO
}


void Interpreter::Start() {
    while (true) {
        int bytecode_index = _FRAME->GetBytecodeIndex();

        pVMMethod method = this->GetMethod();
        uint8_t bytecode = (*method)[bytecode_index];

        int bytecode_length = Bytecode::GetBytecodeLength(bytecode);

        if(dump_bytecodes >1)
            Disassembler::DumpBytecode(_FRAME, method, bytecode_index);

        int next_bytecode_index = bytecode_index + bytecode_length;

        _FRAME->SetBytecodeIndex(next_bytecode_index);
#ifdef __DEBUG
        cout << "Current Bytecode: " << Bytecode::GetBytecodeName(bytecode) 
             << endl;
#endif
// Handle the current bytecode
        switch(bytecode) {
            case BC_HALT:             return; // handle the halt bytecode
            case BC_DUP:              do_dup();  break;
            case BC_PUSH_LOCAL:       do_push_local(bytecode_index); break;
            case BC_PUSH_ARGUMENT:    do_push_argument(bytecode_index); break;
            case BC_PUSH_FIELD:       do_push_field(bytecode_index); break;
            case BC_PUSH_BLOCK:       do_push_block(bytecode_index); break;
            case BC_PUSH_CONSTANT:    do_push_constant(bytecode_index); break;
            case BC_PUSH_GLOBAL:      do_push_global(bytecode_index); break;
            case BC_POP:              do_pop(); break;
            case BC_POP_LOCAL:        do_pop_local(bytecode_index); break;
            case BC_POP_ARGUMENT:     do_pop_argument(bytecode_index); break;
            case BC_POP_FIELD:        do_pop_field(bytecode_index); break;
            case BC_SEND:             do_send(bytecode_index); break;
            case BC_SUPER_SEND:       do_super_send(bytecode_index); break;
            case BC_RETURN_LOCAL:     do_return_local(); break;
            case BC_RETURN_NON_LOCAL: do_return_non_local(); break;
            default:                  _UNIVERSE->ErrorExit(
                                           "Interpreter: Unexpected bytecode"); 
        } // switch
    } // while
}


pVMFrame Interpreter::PushNewFrame( pVMMethod method ) {
    _SETFRAME(_UNIVERSE->NewFrame(_FRAME, method));
    return _FRAME;
}


void Interpreter::SetFrame( pVMFrame frame ) {
    this->frame = frame;   
}


pVMFrame Interpreter::GetFrame() {
    return this->frame;
}


pVMMethod Interpreter::GetMethod() {
    pVMMethod method = _FRAME->GetMethod();
   /* cout << "bytecodes: ";
      for (int i = 0; i < method->BytecodeLength(); ++i)
    {
        cout  << (int)(*method)[i] << " ";
    }
    cout << endl;*/
    return method;
}


pVMObject Interpreter::GetSelf() {
    pVMFrame context = _FRAME->GetOuterContext();//ups...
    return context->GetArgument(0,0);
}


pVMFrame Interpreter::popFrame() {
    pVMFrame result = _FRAME;
    this->SetFrame(_FRAME->GetPreviousFrame());

    result->ClearPreviousFrame();

    return result;
}


void Interpreter::popFrameAndPushResult( pVMObject result ) {
    pVMFrame prevFrame = this->popFrame();

    pVMMethod method = prevFrame->GetMethod();
    int number_of_args = method->GetNumberOfArguments();

    for (int i = 0; i < number_of_args; ++i) _FRAME->Pop();

    _FRAME->Push(result);
}


void Interpreter::send( pVMSymbol signature, pVMClass receiver_class) {
    pVMInvokable invokable = 
                (pVMInvokable) receiver_class->LookupInvokable(signature);

    if (invokable != NULL) {
        (*invokable)(_FRAME);
    } else {
        //doesNotUnderstand
        int number_of_args = Signature::GetNumberOfArguments(signature);

        pVMObject receiver = _FRAME->GetStackElement(number_of_args-1);

        pVMArray arguments_array = _UNIVERSE->NewArray(number_of_args);

        for (int i = number_of_args - 1; i >= 0; --i) {
            pVMObject o = _FRAME->Pop();
            (*arguments_array)[i] = o; //->SetIndexableField(i, o);
        }
        pVMObject arguments[] = { (pVMObject)signature, 
                                  (pVMObject)arguments_array };

        //check if current frame is big enough for this unplanned Send
        //doesNotUnderstand: needs 3 slots, one for this, one for method name, one for args
        int additionalStackSlots = 3 - _FRAME->RemainingStackSize();       
        if (additionalStackSlots > 0) {
#ifdef __DEBUG
            cout << "Creating emergeny frame for doesNotUnderstand:arguments: with " 
                 << additionalStackSlots << " additional stack slots to prevent stack overflow" 
                 << endl;
#endif __DEBUG
            //copy current frame into a bigger one and replace the current frame
            this->SetFrame(VMFrame::EmergencyFrameFrom(_FRAME, additionalStackSlots));
        }

        receiver->Send(dnu, arguments, 2);
    }
}


void Interpreter::do_dup() {
    pVMObject elem = _FRAME->GetStackElement(0);
    _FRAME->Push(elem);
}


void Interpreter::do_push_local( int bytecode_index ) {
    pVMMethod method = _METHOD;
    uint8_t bc1 = (*method)[bytecode_index + 1];
    uint8_t bc2 = (*method)[bytecode_index + 2];

    pVMObject local = _FRAME->GetLocal(bc1, bc2);

    _FRAME->Push(local);
}


void Interpreter::do_push_argument( int bytecode_index ) {
    pVMMethod method = _METHOD;
    uint8_t bc1 = (*method)[bytecode_index + 1];
    uint8_t bc2 = (*method)[bytecode_index + 2];

    pVMObject argument = _FRAME->GetArgument(bc1, bc2);

    _FRAME->Push(argument);
}


void Interpreter::do_push_field( int bytecode_index ) {
    pVMMethod method = _METHOD;

    pVMSymbol field_name = (pVMSymbol) method->GetConstant(bytecode_index);

    pVMObject self = _SELF;
    int field_index = self->GetFieldIndex(field_name);

    pVMObject o = self->GetField(field_index);

    _FRAME->Push(o);
}


void Interpreter::do_push_block( int bytecode_index ) {
    pVMMethod method = _METHOD;

    pVMMethod blockMethod = (pVMMethod)method->GetConstant(bytecode_index);

    int num_of_args = blockMethod->GetNumberOfArguments();

    _FRAME->Push((pVMObject) _UNIVERSE->NewBlock(blockMethod, _FRAME,
                                                 num_of_args));
}


void Interpreter::do_push_constant( int bytecode_index ) {
    pVMMethod method = _METHOD;

    pVMObject constant = method->GetConstant(bytecode_index);
    _FRAME->Push(constant);
    
   // _FRAME->PrintStack();

}


void Interpreter::do_push_global( int bytecode_index) {
    

    pVMMethod method = _METHOD;

    pVMSymbol global_name = (pVMSymbol) method->GetConstant(bytecode_index);

    pVMObject global = _UNIVERSE->GetGlobal(global_name);

    if(global != NULL)
        _FRAME->Push(global);
    else {
        pVMObject arguments[] = { (pVMObject) global_name };
        pVMObject self = _SELF;

        //check if there is enough space on the stack for this unplanned Send
        //unknowGlobal: needs 2 slots, one for "this" and one for the argument
        int additionalStackSlots = 2 - _FRAME->RemainingStackSize();       
        if (additionalStackSlots > 0) {
#ifdef __DEBUG
            cout << "Creating emergeny frame for unknownGlobal: with " 
                 << additionalStackSlots
                 << " additional stack slots to prevent stack overflow" << endl;
#endif __DEBUG
            //copy current frame into a bigger one and replace the current frame
            this->SetFrame(VMFrame::EmergencyFrameFrom(_FRAME,
                           additionalStackSlots));
        }

        self->Send(uG, arguments, 1);
    }
}


void Interpreter::do_pop() {
    _FRAME->Pop();
}


void Interpreter::do_pop_local( int bytecode_index ) {
    pVMMethod method = _METHOD;

    uint8_t bc1 = (*method)[bytecode_index + 1];
    uint8_t bc2 = (*method)[bytecode_index + 2];

    pVMObject o = _FRAME->Pop();

    _FRAME->SetLocal(bc1, bc2, o);
}


void Interpreter::do_pop_argument( int bytecode_index ) {
    pVMMethod method = _METHOD;

    uint8_t bc1 = (*method)[bytecode_index + 1];
    uint8_t bc2 = (*method)[bytecode_index + 2];

    pVMObject o = _FRAME->Pop();
    _FRAME->SetArgument(bc1, bc2, o);
}


void Interpreter::do_pop_field( int bytecode_index ) {
    pVMMethod method = _METHOD;
    pVMSymbol field_name = (pVMSymbol) method->GetConstant(bytecode_index);

    pVMObject self = _SELF;
    int field_index = self->GetFieldIndex(field_name);

    pVMObject o = _FRAME->Pop();
    self->SetField(field_index, o);
}


void Interpreter::do_send( int bytecode_index ) {
    pVMMethod method = _METHOD;
    
    pVMSymbol signature = (pVMSymbol) method->GetConstant(bytecode_index);
#ifdef __DEBUG
        cout << "sig: " << signature->GetChars() << endl;
#endif
    int num_of_args = Signature::GetNumberOfArguments(signature);

    pVMObject receiver = _FRAME->GetStackElement(num_of_args-1);
#ifdef __DEBUG
    cout << "rec("<<num_of_args-1<<"): " << 
            receiver->GetClass()->GetName()->GetChars() << endl;
#endif
    this->send(signature, receiver->GetClass());
}


void Interpreter::do_super_send( int bytecode_index ) {
    pVMMethod method = _METHOD;
    pVMSymbol signature = (pVMSymbol) method->GetConstant(bytecode_index);

    pVMFrame ctxt = _FRAME->GetOuterContext();
    pVMMethod real_method = ctxt->GetMethod();
    pVMClass holder = real_method->GetHolder();
    pVMClass super = holder->GetSuperClass();
    pVMInvokable invokable = (pVMInvokable) super->LookupInvokable(signature);

    if (invokable != NULL)
        (*invokable)(_FRAME);
    else {
        int num_of_args = Signature::GetNumberOfArguments(signature);
        pVMObject receiver = _FRAME->GetStackElement(num_of_args - 1);
        pVMArray arguments_array = _UNIVERSE->NewArray(num_of_args);

        for (int i = num_of_args - 1; i >= 0; --i) {
            pVMObject o = _FRAME->Pop();
            (*arguments_array)[i] = o; //->SetIndexableField(i ,o);
        }
        pVMObject arguments[] = { (pVMObject)signature, 
                                  (pVMObject) arguments_array };
        receiver->Send(dnu, arguments, 2);
    }
}


void Interpreter::do_return_local() {
    pVMObject result = _FRAME->Pop();

    this->popFrameAndPushResult(result);
}


void Interpreter::do_return_non_local() {
    pVMObject result = _FRAME->Pop();

    pVMFrame context = _FRAME->GetOuterContext();

    if (!context->HasPreviousFrame()) {
        pVMBlock block = (pVMBlock) _FRAME->GetArgument(0, 0);
        pVMFrame prev_frame = _FRAME->GetPreviousFrame();
        pVMFrame outer_context = prev_frame->GetOuterContext();
        pVMObject sender = outer_context->GetArgument(0, 0);
        pVMObject arguments[] = { (pVMObject)block };

        this->popFrame();

        sender->Send(eB, arguments, 1);
        return;
    }

    while (_FRAME != context) this->popFrame();

    this->popFrameAndPushResult(result);
}

