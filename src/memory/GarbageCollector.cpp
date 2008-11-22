#include <vector>
#include <map>
#include <string.h>

#include "GarbageCollector.h"
#include "Heap.h"

#include "../vm/Universe.h"

#include "../vmobjects/VMObject.h"
#include "../vmobjects/VMSymbol.h"


GarbageCollector::GarbageCollector(Heap* h) {
	heap = h;
    num_collections = 0;
	num_live = 0;
	spc_live = 0;
	num_freed = 0;
	spc_freed = 0;
    
}


GarbageCollector::~GarbageCollector() {
    //Heap is deleted by Universe
}


void GarbageCollector::Collect() {
    ++num_collections;
	num_live = 0;
	spc_live = 0;
	num_freed = 0;
	spc_freed = 0;
	markReachableObjects();
	void* pointer = heap->object_space;
	free_list_entry* currentEntry = heap->free_list_start;
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
			VMObject* object = (VMObject*) pointer;
			bytesToSkip = object->GetObjectSize();

			if (object->GetGCField() == 1)  {
                ++num_live;
			    spc_live += object->GetObjectSize();
				object->SetGCField(0);
				//std::cout << "Found alive object, keeping" << std::endl;
			} else {
				//std::cout << "Found trash, deleting" << std::endl;
				//delete(object, heap);
                //doesn't really do anything for managed objects
				//add freed space as a new entry of the free list
				memset(pointer, 0, bytesToSkip);
				free_list_entry* newEntry = 
                    reinterpret_cast<free_list_entry*>(pointer);
				++num_freed;
				spc_freed += bytesToSkip;
				newEntry->size = bytesToSkip;
				if (newEntry < heap->free_list_start)  {
					newEntry->next = heap->free_list_start;
					heap->free_list_start = newEntry;
					currentEntry = newEntry;
				} else {
					newEntry->next = currentEntry->next;
					currentEntry->next = newEntry;
				}

			}
		}

		pointer = (void*)((long)pointer + bytesToSkip);

	} while((long)pointer < ((long)(void*)heap->object_space) + 
                                          heap->object_space_size);

	mergeFreeSpaces();

    if(gc_verbosity > 1)
        this->PrintCollectStat();
    if(gc_verbosity > 2) {
        cerr << "TODO: dump heap" << endl;
        //cerr << "-- post-collection heap dump --" << endl;

        //gc_show_memory();
    }
}


void GarbageCollector::markReachableObjects() {
	map<pString, VMObject*> globals = Universe::GetUniverse()->GetGlobals();

	for (map<pString, VMObject*>::iterator it = globals.begin(); 
                                        it!= globals.end(); ++it) {
		markObject(&(*it->second));
	}
    // Get the current frame and mark it.
	// Since marking is done recursively, this automatically
	// marks the whole stack
    VMFrame* current_frame = _UNIVERSE->GetInterpreter()->GetFrame();
    if (current_frame != NULL) {
        markObject((VMObject*)current_frame);
    }
}


void GarbageCollector::markObject(VMObject* obj) {
	if (   ((void*) obj >= (void*)  heap->object_space) 
		&& ((void*) obj <= (void*) heap->object_space) + heap->object_space_size) {
		if (obj->GetGCField() != 1) {
			//++num_live;
			//spc_live += obj->GetObjectSize();

			/*obj->SetGCField(1);*/
			//for now the Objects have to mark the referenced objects themselves.
			obj->MarkReferences();
			
		}
	}
}


void GarbageCollector::mergeFreeSpaces() {
#ifdef __DEBUG
	std::cout << "free heap before collecting: " << heap->size_of_free_heap << std::endl;
#endif __DEBUG

	free_list_entry* currentEntry = heap->free_list_start;
	heap->size_of_free_heap = 0;
	while (currentEntry->next != NULL) {
		if((int)currentEntry + (int)currentEntry->size == 
                                        (int)currentEntry->next) {
			currentEntry->size += currentEntry->next->size; 
			currentEntry->next = currentEntry->next->next;
		} else {
			heap->size_of_free_heap += currentEntry->size;
			currentEntry = currentEntry->next;
		}
	}
	heap->size_of_free_heap += currentEntry->size;

#ifdef __DEBUG
	std::cout << "free heap after collecting: " << heap->size_of_free_heap << std::endl;
#endif
}

#define _KB(B) (B/1024)
#define _MB(B) ((double)B/(1024.0*1024.0))

void GarbageCollector::PrintGCStat() {
    cerr << "-- GC statistics --" << endl;
    cerr << "* heap size " << heap->object_space_size << " B (" << 
        _KB(heap->object_space_size) << " kB, " << 
        _MB(heap->object_space_size) << " MB)" << endl;
    cerr << "* performed " << num_collections << " collections" << endl;
}

void GarbageCollector::PrintCollectStat() {
    cerr << endl << "[GC " << num_collections << ", " << 
        heap->num_alloc << " alloc (" << _KB(heap->spc_alloc) <<
        " kB), " << num_live << " live (" << _KB(spc_live) <<
        " kB), " << num_freed << " freed (" <<  _KB(spc_freed) <<
        " kB)]" << endl;
}

