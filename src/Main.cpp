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
#include "vmobjects/VMPointer.h"
#include "vmobjects/ObjectFormats.h"

int main(int argc, char** argv) {

    cout << "This is CppSOM.\n" << endl;

   // int vm_argc = 0;
   // StdString* vm_argv = NULL;

   // Universe::handleArguments(&vm_argc, argc, argv);
    Universe::Start(argc, argv);

    cout << "pVMObject size: " << sizeof(VMPointer<VMObject>) << endl;
    VMArray* o = _UNIVERSE->NewArray(5);
    pVMArray array;
    array = o;
    pVMObject object;
    object = array;
    pVMString string = _UNIVERSE->NewString("testString");
    object = string;
    cout << "Accessing testString via cast from pVMObject<VMObject> "
         << "to pVMObject<VMString>: " <<((VMPointer<VMString>)object)->GetChars()
         << endl;
    object = o;
    if (object == array) cout << "ja pVMArray(o) == pVMObject(o)" << endl;
    if (object != string) cout << "!= ist auch toll" << endl;

    pVMInteger ti;
    pVMInteger ti2;
    ti = 8;
    ti2 = 8;
    if (ti == ti2) cout << "8 == 8" << endl;
    //po->SetIndexableField(0, ti);
    (*array)[0] = ti;
    //po->operator [](0);
    pVMInteger ti3((VMInteger*)(*array)[0]);
    cout << "(int32_t)ti3 + 9 = " << (int32_t)ti3 + 9 << endl;
    cout << "4 + (int32_t)ti = " << 4 + (int32_t)ti << endl;
    cout << "ti->GetObjectSize(): " << ti->GetObjectSize() << endl;
    Universe::Quit(ERR_SUCCESS);
}


