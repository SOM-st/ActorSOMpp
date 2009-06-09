#pragma once
#ifndef INTERPRETER_H_
#define INTERPRETER_H_

/*
 *
 *
Copyright (c) 2007 Michael Haupt, Tobias Pape, Arne Bergmann
Software Architecture Group, Hasso Plattner Institute, Potsdam, Germany
http://www.hpi.uni-potsdam.de/swa/

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
  */


#include "../misc/defs.h"

#include "../vmobjects/ObjectFormats.h"
#include "../vmobjects/VMPointer.h"

#include "../actors/GlobalObjectId.h"
//#include "../actors/messages.h"

class VMMethod;
class VMFrame;
class VMMethod;
class VMObject;
class VMProtoObject;
class VMSymbol;
class VMClass;

class SomMessage;
class SomMessageWithResult;

class Interpreter {
public:
    Interpreter();
    ~Interpreter();
    void Start();
    void ProcessActivations();
    pVMFrame PushNewFrame(pVMMethod method);
    void SetFrame(pVMFrame frame);
    pVMFrame GetFrame();
    pVMMethod GetMethod();
    pVMObject GetSelf();
    
    void Stop();
    void ProcessIncommingMessages();
    
    void WaitForObjectReference();
    
    
    static void AddIncommingObjRef(pVMObject obj);
    static void HandleRemoteReturn(GlobalObjectId waitingActivation, pVMObject result);
    static void ProcessMessage(SomMessage* msg);
    static void ProcessMessage(SomMessageWithResult* msg);
    
private:
    pVMFrame frame;
    StdString uG;
    StdString dnu;
    StdString eB;
    
    bool stop;

    pVMFrame popFrame();
    void popFrameAndPushResult(pVMObject result);
    void send(pVMSymbol signature, pVMClass receiverClass);
    void sendSyncMessage(pVMObject receiver, pVMSymbol signature, 
                         size_t numOfArgs, pVMFrame currentFrame);
    void setNextActivation();
    
    void do_DUP(int bytecodeIndex);
    void do_PUSH_LOCAL(int bytecodeIndex);
    void do_PUSH_ARGUMENT(int bytecodeIndex);
    void do_PUSH_FIELD(int bytecodeIndex);
    void do_PUSH_BLOCK(int bytecodeIndex);
    void do_PUSH_CONSTANT(int bytecodeIndex);
    void do_PUSH_GLOBAL(int bytecodeIndex);
    void do_POP(int bytecodeIndex);
    void do_POP_LOCAL(int bytecodeIndex);
    void do_POP_ARGUMENT(int bytecodeIndex);
    void do_POP_FIELD(int bytecodeIndex);
    void do_SEND(int bytecodeIndex);
    void do_SUPER_SEND(int bytecodeIndex);
    void do_RETURN_LOCAL(int bytecodeIndex);
    void do_RETURN_NON_LOCAL(int bytecodeIndex);

    void do_SEND_ASYNC(int bytecodeIndex);
    void do_YIELD(int bytecodeIndex);
    void do_RETURN_REMOTE(int bytecodeIndex);

};

#endif
