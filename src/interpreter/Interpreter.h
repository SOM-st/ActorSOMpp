#pragma once

#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include "../misc/defs.h"

#include "../vmobjects/ObjectFormats.h"

class VMMethod;
class VMFrame;
class VMMethod;
class VMObject;
class VMSymbol;
class VMClass;

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
    StdString uG;
    StdString dnu;
    StdString eB;

    pVMFrame popFrame();
    void popFrameAndPushResult(pVMObject result);
    void send(pVMSymbol signature, pVMClass receiverClass);
    
    void doDup();
    void doPushLocal(int bytecodeIndex);
    void doPushArgument(int bytecodeIndex);
    void doPushField(int bytecodeIndex);
    void doPushBlock(int bytecodeIndex);
    void doPushConstant(int bytecodeIndex);
    void doPushGlobal(int bytecodeIndex);
    void doPop(void);
    void doPopLocal(int bytecodeIndex);
    void doPopArgument(int bytecodeIndex);
    void doPopField(int bytecodeIndex);
    void doSend(int bytecodeIndex);
    void doSuperSend(int bytecodeIndex);
    void doReturnLocal();
    void doReturnNonLocal();
};

#endif
