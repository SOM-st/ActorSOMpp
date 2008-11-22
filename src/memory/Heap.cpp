#include <iostream>
#include <stdlib.h>
#include <string.h>

#include "Heap.h"

#include "../vmobjects/VMObject.h"
#include "../vm/Universe.h"

Heap* Heap::theHeap = NULL;

Heap* Heap::GetHeap() {
    if (!theHeap) {
        _UNIVERSE->ErrorExit("Trying to access uninitialized Heap");
    }
    return theHeap;
}

void Heap::InitializeHeap( int objectSpaceSize ) {
    if (theHeap) {
        cout << "Warning, reinitializing already initialized Heap, " 
             << "all data will be lost!" << endl;
        delete theHeap;
    }
    theHeap = new Heap(objectSpaceSize);
}

void Heap::DestroyHeap() {
    if (theHeap) delete theHeap;
}



Heap::Heap(int object_space_size) {
	object_space = (void*) malloc(object_space_size);
	if (!object_space) {
		std::cout << "Failed to allocate the initial "<< object_space_size 
                  << " bytes for the GC. Panic.\n" << std::endl;
		exit(1);
	}
	memset(object_space, 0, object_space_size);
	size_of_free_heap = object_space_size;
	this->object_space_size = object_space_size;
	this->buffersize_for_uninterruptable = (int) (object_space_size * 0.1);
    
    uninterruptable_counter = 0;
	num_alloc = 0;
    spc_alloc = 0;

	free_list_start = (free_list_entry*) object_space;
	free_list_start->size = object_space_size;
	free_list_start->next = NULL;
	gc = new GarbageCollector(this);
}

Heap::~Heap() {
    if (gc_verbosity > 0)
        gc->PrintGCStat();
	free(object_space);
}

VMObject* Heap::AllocateObject(size_t size) {
    VMObject* vmo = (VMObject*) Allocate(size);
    vmo->SetObjectSize(size);
    ++num_alloc;
    spc_alloc += size;
    return vmo;
}

void* Heap::Allocate(size_t size) {
	if (size == 0) return NULL;
    if (size < sizeof(free_list_entry))  {
        return internalAllocate(size);
    }
#ifdef HEAPDEBUG 
    std::cout << "allocating: " << (int)size << "bytes" << std::endl;
#endif
	if (size_of_free_heap <= buffersize_for_uninterruptable &&
		uninterruptable_counter <= 0)  {
#ifdef HEAPDEBUG
        cout << "Not enough free memory, only: " << size_of_free_heap 
             << " bytes left." << endl
             << "Starting Garbage Collection" << endl;
#endif
		gc->Collect();

        //
        //reset allocation stats
        //
        num_alloc = 0;
        spc_alloc = 0;
	}
	
	void* result = NULL;
	free_list_entry* current_entry = free_list_start;
	free_list_entry* before_entry = NULL;

    //
	//find first fit
    //
	while (! ((current_entry->size == size) 
               || (current_entry->next == NULL) 
               || (current_entry->size >= (size + sizeof(free_list_entry))))) { 
        before_entry = current_entry;
        current_entry = current_entry->next;
    }
	
    //
	// did we find a perfect fit?
	// if so, we simply remove this entry from the list
    //
    if (current_entry->size == size) {
        if (current_entry == free_list_start) { 
			// first one fitted - adjust the 'first-entry' pointer
            
            free_list_start = current_entry->next; 
			//PROBLEM (also in CSOM?): 
            //what if last possible allocate is a perfect fit?
        } else {
			// simply remove the reference to the found entry
            before_entry->next = current_entry->next;
        } // entry fitted
        result = current_entry;
		
    } else {
		// did we find an entry big enough for the request and a new
		// free_entry?
        if (current_entry->size >= (size + sizeof(free_list_entry))) {
            // save data from found entry
            int old_entry_size = current_entry->size;
            free_list_entry* old_next = current_entry->next;
            
            result = current_entry;
            // create new entry and assign data
            free_list_entry* replace_entry =  
                            (free_list_entry*) ((int)current_entry + size);
            
            replace_entry->size = old_entry_size - size;
            replace_entry->next = old_next;
            if (current_entry == free_list_start) {
                free_list_start = replace_entry;
            } else {
                before_entry->next = replace_entry;
            }
        }  else { 
			// no space was left
			// running the GC here will most certainly result in data loss!
//#ifdef HEAPDEBUG 
            cout << "Not enough heap! Data loss is possible" << endl
			          << "FREE-Size: " << size_of_free_heap 
                      << ", uninterruptable_counter: " 
                      << uninterruptable_counter << endl;
//#endif
            
			gc->Collect();
            num_alloc = 0;
            spc_alloc = 0;

            //fulfill initial request
            result = Allocate(size);
        }
    }
           
    if(!result) {
		cout << "Failed to allocate "<< (int)size <<" bytes. Panic." << endl;
        //Universe_exit(-1);
		exit(1);
    }
    memset(result, 0, size);

#ifdef HEAPDEBUG 
    cout << "available heap size before alloc: " << size_of_free_heap << endl;
#endif
	// update the available size
    size_of_free_heap -= size;

#ifdef HEAPDEBUG 
    cout << "available heap size after alloc: " << size_of_free_heap << endl;
    cout << "heap-start: " << hex  << object_space << endl;
    cout << "allocated at address: " << hex << result << endl;
#endif
    return result;
}

void Heap::Free(void* ptr) {
    if ( ((int)ptr < (int) this->object_space) &&
        ((int)ptr > (int) this->object_space + this->object_space_size)) {
        internalFree(ptr);
    }
}

void Heap::Free(void* /*ptr*/, int /*size*/) {
    //add referenced space to free list
}

void Heap::internalFree(void* ptr) {
    free(ptr);
}

void* Heap::internalAllocate(size_t size) {
    if (size == 0) return NULL;
    void* result = malloc(size);
    if(!result) {
        cout << "Failed to allocate " << size << " Bytes." << endl;
        _UNIVERSE->Quit(-1);
    }
    memset(result, 0, size);
    return result;
}
