#pragma once

#ifndef SHELL_H_
#define SHELL_H_

class VMMethod;

class Shell {
public:
    Shell();
    Shell(pVMMethod);
    ~Shell();
    void SetBootstrapMethod(pVMMethod bsm) { bootstrapMethod = bsm; } ;
    pVMMethod GetBootstrapMethod() const { return bootstrapMethod; } ;
    void Start();
private:
    pVMMethod bootstrapMethod;
};

#endif
