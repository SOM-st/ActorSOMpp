#pragma once

#ifndef SHELL_H_
#define SHELL_H_

class VMMethod;

class Shell
{
public:
    Shell();
    Shell(VMMethod*);
    ~Shell();
    void SetBootstrapMethod(VMMethod* bsm) { bootstrap_method = bsm; } ;
    VMMethod* GetBootstrapMethod() { return bootstrap_method; } ;
    void Start();
private:
    VMMethod* bootstrap_method;
};

#endif