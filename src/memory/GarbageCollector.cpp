#include <vector>
#include <map>
#include <string.h>

#include "GarbageCollector.h"
#include "Heap.h"

#include "../vm/Universe.h"
#include "../vmobjects/VMMethod.h"
#include "../vmobjects/VMObject.h"
#include "../vmobjects/VMSymbol.h"


GarbageCollector::GarbageCollector(Heap* h) {
	heap = h;
    numCollections = 0;
	numLive = 0;
	spcLive = 0;
	numFreed = 0;
	spcFreed = 0;
    
}


GarbageCollector::~GarbageCollector() {
    //Heap is deleted by Universe
}


void GarbageCollector::Collect() {
    ++numCollections;
	numLive = 0;
	spcLive = 0;
	numFreed = 0;
	spcFreed = 0;
	markReachableObjects();
	void* pointer = heap->objectSpace;
	FreeListEntry* currentEntry = heap->freeListStart;
	long bytesToSkip = 0;
#ifdef __DEBUG
    cout << "Starting garbage collection." << endl;
#endif
	do {
		//see whether the currentObject is part of the free list
		while( currentEntry->next != NULL &&
			pointer > (void*)currentEntry->next) {
			currentEntry = currentEntry->next;
		}

		//check if it is in the free list
		if (pointer == (void*)currentEntry) {
			bytesToSkip = currentEntry->size;
		} else if (pointer == (void*)currentEntry->next)  {
			bytesToSkip = currentEntry->next->size;
		} else { //we found a VMObject
			pVMObject object = (pVMObject) pointer;
			bytesToSkip = object->GetObjectSize();
            
			if (object->GetGCField() == 1)  {
                ++numLive;
			    spcLive += object->GetObjectSize();
				object->SetGCField(0);
				//std::cout << "Found alive object, keeping" << std::endl;
			} else {
				//std::cout << "Found trash, deleting" << std::endl;
				//delete(object, heap);
                //doesn't really do anything for managed objects
				//add freed space as a new entry of the free list
				memset(pointer, 0, bytesToSkip);
				FreeListEntry* newEntry = 
                    reinterpret_cast<FreeListEntry*>(pointer);
				++numFreed;
				spcFreed += bytesToSkip;
				newEntry->size = bytesToSkip;
				if (newEntry < heap->freeListStart)  {
					newEntry->next = heap->freeListStart;
					heap->freeListStart = newEntry;
					currentEntry = newEntry;
				} else {
					newEntry->next = currentEntry->next;
					currentEntry->next = newEntry;
				}

			}
		}

		pointer = (void*)((long)pointer + bytesToSkip);

	} while((long)pointer < ((long)(void*)heap->objectSpace) + 
                                          heap->objectSpaceSize);

	mergeFreeSpaces();

    if(gcVerbosity > 1)
        this->PrintCollectStat();
    if(gcVerbosity > 2) {
        cerr << "TODO: dump heap" << endl;
    }
}


void GarbageCollector::markReachableObjects() {
	map<pVMSymbol, pVMObject> globals = Universe::GetUniverse()->GetGlobals();
    map<pVMSymbol, pVMObject>::iterator it = globals.begin();
    for (map<pVMSymbol, pVMObject>::iterator it = globals.begin(); 
                                        it!= globals.end(); ++it) {
		markObject(&(*it->second));
	}
    // Get the current frame and mark it.
	// Since marking is done recursively, this automatically
	// marks the whole stack
    pVMFrame currentFrame = _UNIVERSE->GetInterpreter()->GetFrame();
    if (currentFrame != NULL) {
        markObject((pVMObject)currentFrame);
    }
}


void GarbageCollector::markObject(pVMObject obj) {
	if (   ((void*) obj >= (void*)  heap->objectSpace) 
		&& ((void*) obj <= (void*) heap->objectSpace) + heap->objectSpaceSize) {
		if (obj->GetGCField() != 1) {
			//++numLive;
			//spcLive += obj->GetObjectSize();

			/*obj->SetGCField(1);*/
			//for now the Objects have to mark the referenced objects themselves.
			obj->MarkReferences();
			
		}
	}
}


void GarbageCollector::mergeFreeSpaces() {

	FreeListEntry* currentEntry = heap->freeListStart;
	heap->sizeOfFreeHeap = 0;
	while (currentEntry->next != NULL) {
		if((int)currentEntry + (int)currentEntry->size == 
                                        (int)currentEntry->next) {
			currentEntry->size += currentEntry->next->size; 
			currentEntry->next = currentEntry->next->next;
		} else {
			heap->sizeOfFreeHeap += currentEntry->size;
			currentEntry = currentEntry->next;
		}
	}
	heap->sizeOfFreeHeap += currentEntry->size;

}

#define _KB(B) (B/1024)
#define _MB(B) ((double)B/(1024.0*1024.0))

void GarbageCollector::PrintGCStat() const {
    cerr << "-- GC statistics --" << endl;
    cerr << "* heap size " << heap->objectSpaceSize << " B (" << 
        _KB(heap->objectSpaceSize) << " kB, " << 
        _MB(heap->objectSpaceSize) << " MB)" << endl;
    cerr << "* performed " << numCollections << " collections" << endl;
}

void GarbageCollector::PrintCollectStat() const {
    cerr << endl << "[GC " << numCollections << ", " << 
        heap->numAlloc << " alloc (" << _KB(heap->spcAlloc) <<
        " kB), " << numLive << " live (" << _KB(spcLive) <<
        " kB), " << numFreed << " freed (" <<  _KB(spcFreed) <<
        " kB)]" << endl;
}

