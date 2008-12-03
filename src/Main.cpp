/*
 * $Id: main.c 523 2008-05-28 18:08:31Z michael.seegers $
 *
Copyright (c) 2007 Michael Haupt, Tobias Pape
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

//#include <memory/gc.h>

//#include <vm/Universe.h>
//#include <stdio.h>

#include <iostream>
#include <fstream>

#include "compiler/ClassGenerationContext.h"

#include "memory/Heap.h"

#include "misc/ExtendedList.h"
#include "misc/defs.h"

#include "vm/Universe.h"

#include "vmobjects/VMObject.h"
#include "vmobjects/VMMethod.h"
#include "vmobjects/VMString.h"
#include "vmobjects/VMArray.h"
#include "vmobjects/pVMObject.h"

int main(int argc, char** argv) {

    cout << "This is CppSOM.\n" << endl;

   // int vm_argc = 0;
   // StdString* vm_argv = NULL;

   // Universe::handleArguments(&vm_argc, argc, argv);
    Universe::Start(argc, argv);
    
    VMArray* o = _UNIVERSE->NewArray(5);
    pVMObject<VMArray> po;
    po = o;
    
    cout << "pVMObject size: " << sizeof(pVMObject<VMObject>) << endl;
    pVMObject<VMInteger> ti;
    ti = 8;
    po->SetIndexableField(0, ti);
    po->operator [](0);
    pVMObject<VMInteger> ti2((VMInteger*)(po->operator [](0)));
    cout << "ti + 9 = " << (int32_t)ti + 9 << endl;

    Universe::Quit(ERR_SUCCESS);
}


