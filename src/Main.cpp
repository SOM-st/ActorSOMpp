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
#include "compiler/ClassGenerationContext.h"
#include <iostream>
#include <fstream>

#include <typeinfo>
//#include "compiler/Parser.h"
#include "memory/Heap.h"
#include "vmobjects/VMObject.h"
#include "vmobjects/VMMethod.h"
#include "misc/ExtendedList.h"
#include "vm/Universe.h"
#include "misc/HeapAllocator.h"
#include "vmobjects/VMString.h"
#include "vmobjects/VMArray.h"
#include "misc/defs.h"
#include "vmobjects/pVMObject.h"

int main(int argc, char** argv) {

    cout << "This will be CppSOM.\n" << endl;

   // int vm_argc = 0;
   // pString* vm_argv = NULL;

   // Universe::handleArguments(&vm_argc, argc, argv);
    Universe::Start(argc, argv);
    
    /*pVMObject pti;
    pti = _UNIVERSE->NewTaggedInteger(42);
    pVMObject pti2;
    pti2 = (VMObject*) _UNIVERSE->NewBigInteger(123490123);

    cout << sizeof(pVMObject) << endl;*/
   /* Heap* heap = new Heap(1000);
    VMObject* vmo = new (heap) VMObject();
    cout << "sizeof(VMObject): " << sizeof(VMObject) << endl;*/

    Universe::Quit(ERR_SUCCESS);
}


