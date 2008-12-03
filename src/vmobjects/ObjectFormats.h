#pragma once

#ifndef OBJECTFORMATS_H_
#define OBJECTFORMATS_H_
class VMArray;
class VMBigInteger;
class VMSymbol;
class VMDouble;
class VMEvaluationPrimitive;
class VMFrame;
class VMInteger;
class VMInvokable;
class VMMethod;
class VMObject;
class VMPrimitive;
class VMString;
class VMSymbol;

typedef VMPointer<VMArray>      pVMArray;
typedef VMPointer<VMBigInteger> pVMBigInteger;
typedef VMPointer<VMBlock>      pVMBlock;
typedef VMPointer<VMClass>      pVMClass;
typedef VMPointer<VMDouble>     pVMDouble;
typedef VMPointer<VMEvaluationPrimitive> pVMEvaluationPrimitive;
typedef VMPointer<VMFrame>      pVMFrame;
typedef VMPointer<VMInteger>    pVMInteger;
typedef VMPointer<VMInvokable>  pVMInvokable;
typedef VMPointer<VMMethod>     pVMMethod;
typedef VMPointer<VMObject>     pVMObject;
typedef VMPointer<VMPrimitive>  pVMPrimitive;
typedef VMPointer<VMString>     pVMString;
typedef VMPointer<VMSymbol>     pVMSymbol;

#endif OBJECTFORMATS_H_
