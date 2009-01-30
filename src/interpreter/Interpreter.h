#pragma once

#ifndef INTERPRETER_H_
#define INTERPRETER_H_

class VMMethod;
class VMFrame;
class VMMethod;
class VMObject;
class VMSymbol;
class VMClass;

#include "../misc/defs.h"
#include "../vmobjects/ObjectFormats.h"

class Interpreter {
public:
    Interpreter();
    ~Interpreter();
    void Start();
    pVMFrame PushNewFrame(pVMMethod method);
    void SetFrame(pVMFrame frame);
    pVMFrame GetFrame();
    pVMMethod GetMethod();
    pVMObject GetSelf();
private:
    pVMFrame frame;
    pVMFrame popFrame();
    StdString uG;
    StdString dnu;
    StdString eB;

    void popFrameAndPushResult(pVMObject result);
    void send(pVMSymbol signature, pVMClass receiver_class);
    
    void do_dup();
    void do_push_local(int bytecode_index);
    void do_push_argument(int bytecode_index);
    void do_push_field(int bytecode_index);
    void do_push_block(int bytecode_index);
    void do_push_constant(int bytecode_index);
    void do_push_global(int bytecode_index);
    void do_pop(void);
    void do_pop_local(int bytecode_index);
    void do_pop_argument(int bytecode_index);
    void do_pop_field(int bytecode_index);
    void do_send(int bytecode_index);
    void do_super_send(int bytecode_index);
    void do_return_local();
    void do_return_non_local();
};

#endif
