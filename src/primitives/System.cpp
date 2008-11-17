/*
 * $Id: System.c 172 2008-01-15 15:18:55Z tobias.pape $
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

#include <stdio.h>

#include <time.h>

#include <vmobjects/VMObject.h>
#include <vmobjects/VMFrame.h>
#include <vmobjects/VMString.h>
#include <vmobjects/VMInteger.h>

#include <vm/Universe.h>
#include "Core.h"
#include "System.h"
#include "Routine.h"

#if defined(__GNUC__)

    #include <sys/time.h>

#else

    #include <misc/gettimeofday.h>

#endif

_System* System_;

void  _System::Global_(VMObject* /*object*/, VMFrame* frame) {
    VMSymbol* arg = (VMSymbol*)frame->Pop();
    /*VMObject* self = */
    frame->Pop();
    VMObject* result = universe->GetGlobal(arg);
    
    frame->Push( result ? result : Globals::NilObject());    
}


void  _System::Global_put_(VMObject* /*object*/, VMFrame* frame) {
    VMObject* value = frame->Pop();
    VMSymbol* arg = (VMSymbol*)frame->Pop();
    universe->SetGlobal(arg, value);    
}


void  _System::Load_(VMObject* /*object*/, VMFrame* frame) {
    VMSymbol* arg = (VMSymbol*)frame->Pop();
    /*VMObject* self = */
    frame->Pop();
    VMClass* result = universe->LoadClass(arg);

    frame->Push( result? (VMObject*)result : Globals::NilObject());
}


void  _System::Exit_(VMObject* /*object*/, VMFrame* frame) {
    VMInteger* err = (VMInteger*)frame->Pop();
    int32_t err_no = err->GetEmbeddedInteger();

    if(err_no != ERR_SUCCESS)
        frame->PrintStackTrace();
    universe->Quit(err_no);
}


void  _System::PrintString_(VMObject* /*object*/, VMFrame* frame) {
    VMString* arg = (VMString*)frame->Pop();
    pString str = arg->GetStdString();
    cout << str;
}


void  _System::PrintNewline(VMObject* /*object*/, VMFrame* /*frame*/) {
    cout << endl;   
}


void  _System::Time(VMObject* /*object*/, VMFrame* frame) {
    /*VMObject* self = */
    frame->Pop();
    timeval* now = new timeval();

    gettimeofday(now, NULL);

    long long diff = 
        ((now->tv_sec - start_time->tv_sec) * 1000) + //seconds
        ((now->tv_usec - start_time->tv_usec) / 1000); // µseconds

    frame->Push((VMObject*)universe->NewInteger((int32_t)diff));

    delete(now);
}


_System::_System(void) {
    start_time = new timeval();
     gettimeofday(start_time, NULL);
}

_System::~_System()
{
    delete(start_time);
}

PrimitiveRoutine* _System::GetRoutine( const pString& routineName )
{
    PrimitiveRoutine* result;
    if (routineName == pString("Global_"))
        result = new (heap) Routine<_System>(System_, &_System::Global_);
    else if (routineName == pString("Global_put_"))
        result = new (heap) Routine<_System>(System_, &_System::Global_put_);
    else if (routineName == pString("Load_"))
        result = new (heap) Routine<_System>(System_, &_System::Load_);
    else if (routineName == pString("Exit_"))
        result = new (heap) Routine<_System>(System_, &_System::Exit_);
    else if (routineName == pString("PrintString_"))
        result = new (heap) Routine<_System>(System_, &_System::PrintString_);
    else if (routineName == pString("PrintNewline"))
        result = new (heap) Routine<_System>(System_, &_System::PrintNewline);
    else if (routineName == pString("Time"))
        result = new (heap) Routine<_System>(System_, &_System::Time);
    else {
        cout << "method " << routineName << "not found in class System" << endl;
        return NULL;
    }
    return result;
}

