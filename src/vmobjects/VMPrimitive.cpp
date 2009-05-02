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


#include "VMPrimitive.h"
#include "VMSymbol.h"
#include "VMClass.h"

#include "../vm/Universe.h"

//needed to instanciate the Routine object for the  empty routine
#include "../primitivesCore/Routine.h"


pVMPrimitive VMPrimitive::GetEmptyPrimitive( pVMSymbol sig ) {
    
    pVMPrimitive prim = new (_HEAP) VMPrimitive(sig);
    prim->empty = (bool*)1;
    prim->SetRoutine(new Routine<VMPrimitive>(prim, &VMPrimitive::EmptyRoutine));
    return prim;
}

const int VMPrimitive::VMPrimitiveNumberOfFields = 2; 

VMPrimitive::VMPrimitive(pVMSymbol signature) : VMInvokable(VMPrimitiveNumberOfFields) {
    _HEAP->StartUninterruptableAllocation();
    //the only class that explicitly does this.
    this->SetClass(primitiveClass);
    
    this->SetSignature(signature);
    this->routine = NULL;
    this->empty = (bool*)0;

    _HEAP->EndUninterruptableAllocation();
}



void VMPrimitive::MarkReferences() {
    if (gcfield) return;

    this->SetGCField(1);
    //explicitly implement marking for primitives, as they include fields
    //that should not be marked!!!
    for( int i = 0; i < this->GetNumberOfFields(); ++i) {
        //HACK to avoid calling MarkReferences() for the bool*
        if ((void*)GetField(i) == (void*)this->empty) 
            continue;
        //HACK to avoid calling MarkReferences() for the routine*
        if ((void*)GetField(i) == (void*)this->routine)
            continue;
        GetField(i)->MarkReferences();
    }
}


void VMPrimitive::EmptyRoutine( pVMObject _self, pVMFrame /*frame*/ ) {
    pVMInvokable self = (pVMInvokable)( _self );
    pVMSymbol sig = self->GetSignature();
    cout << "undefined primitive called: " << sig->GetChars() << endl;
}


