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
//#include "compiler/Parser.h"
#include "memory/Heap.h"
#include "vmobjects/VMObject.h"
#include "vmobjects/VMMethod.h"
#include "misc/ExtendedList.h"
#include "vm/Universe.h"
#include "misc/HeapAllocator.h"
#include "vmobjects/VMString.h"

int main(int argc, char** argv) {

    cout << "This will be CppSOM.\n" << endl;
	Universe* uni = Universe::GetUniverse();
	Heap *heap = uni->GetHeap();
	//while (1) {
		VMObject *vmo = new (heap) VMObject;
		VMObject *vmo2 = new (heap) VMMethod;
	//}
	vector<VMObject*, HeapAllocator<VMObject*> > v;
	v.push_back(vmo);
	v.push_back(vmo2);

	VMString *vmstr = new (heap) VMString;
	vmstr->FromCString("VMString Test");
	cout << vmstr->ToCString() << endl;
	//cout << vmo->getObjectSize() << endl;
	//cout << vmo2->getObjectSize() << endl;
	
	//uni->RunGC();

	/*ExtendedList *list = new ExtendedList();
	list->Add(vmo);
	list->Add(vmo2);
	cout << "List-size:" << list->Size() << endl;*/
	
	//if (argc < 2) {
	//	cout << "Please specify the file(s) you'd like to parse" << endl;
	//	cout << "Usage: cppsom [files]" << endl;
	//	return -1;
	//}
	//
	//for (int i = 1; i < argc; i++) { 
	//	ifstream fp;
	//	fp.open(argv[i], std::ios_base::in);
	//	if (!fp.is_open()) {
	//		cout << "error opening " << argv[i] <<endl;
	//		continue;
	//	}
	//	//int cdc = 1;
	//	class_generation_context cdc;
	//	cout << "starting " << argv[i] << endl;
	//	Parser* p = new Parser(fp);
	//	p->Classdef(&cdc);

	//	//clean up
	//	fp.close();
	//	delete(p);
	//	
	//	cout << "finished " << argv[i] << endl;

	//}
}


