#pragma once

#ifndef SHELL_H_
#define SHELL_H_

class VMMethod;

class Shell {
public:
    Shell();
    Shell(pVMMethod);
    ~Shell();
    void SetBootstrapMethod(pVMMethod bsm) { bootstrap_method = bsm; } ;
    pVMMethod GetBootstrapMethod() const { return bootstrap_method; } ;
    void Start();
private:
    pVMMethod bootstrap_method;
};

#endif
