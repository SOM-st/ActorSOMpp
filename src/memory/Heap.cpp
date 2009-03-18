#include <iostream>
#include <stdlib.h>
#include <string.h>

#include "Heap.h"

#include "../vmobjects/VMObject.h"
#include "../vm/Universe.h"

/*
 * macro for padding - only word-aligned memory must be allocated
 */
#define PAD_BYTES(N) ((sizeof(void*) - ((N) % sizeof(void*))) % sizeof(void*))

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



Heap::Heap(int objectSpaceSize) {
	objectSpace = (void*) malloc(objectSpaceSize);
	if (!objectSpace) {
		std::cout << "Failed to allocate the initial "<< objectSpaceSize 
                  << " bytes for the GC. Panic.\n" << std::endl;
		exit(1);
	}
	memset(objectSpace, 0, objectSpaceSize);
	sizeOfFreeHeap = objectSpaceSize;
	this->objectSpaceSize = objectSpaceSize;
	this->buffersizeForUninterruptable = (int) (objectSpaceSize * 0.1);
    
    uninterruptableCounter = 0;
	numAlloc = 0;
    spcAlloc = 0;
    numAllocTotal = 0;

	freeListStart = (FreeListEntry*) objectSpace;
	freeListStart->size = objectSpaceSize;
	freeListStart->next = NULL;
	gc = new GarbageCollector(this);
}

Heap::~Heap() {
    if (gcVerbosity > 0) {
        cout << "-- Heap statistics --" << endl;
        cout << "Total number of allocations: " << numAllocTotal << endl;
        cout << "Number of allocations since last collection: " 
             << numAlloc << endl;
        std::streamsize p = cout.precision();
        cout.precision(3);
        cout << "Used memory: " << spcAlloc << "/" 
             << this->objectSpaceSize << " (" 
             << ((double)spcAlloc/(double)this->objectSpaceSize)*100 << "%)" << endl;
        cout.precision(p);
        gc->PrintGCStat();
    }
	free(objectSpace);
    
}

pVMObject Heap::AllocateObject(size_t size) {
    //add padding, so objects are word aligned
    size_t paddedSize = size + PAD_BYTES(size);
    pVMObject vmo = (pVMObject) Allocate(paddedSize);
    //Problem: setting the objectSize here doesn't work if the allocated
    //object is either VMMethode or VMPrimitive because of the multiple inheritance
    vmo->SetObjectSize(paddedSize);
    ++numAlloc;
    ++numAllocTotal;
    spcAlloc += paddedSize;
    return vmo;
}

void* Heap::Allocate(size_t size) {
	if (size == 0) return NULL;
    if (size < sizeof(FreeListEntry))  {
        return internalAllocate(size);
    }
#ifdef HEAPDEBUG 
    std::cout << "allocating: " << (int)size << "bytes" << std::endl;
#endif
	if (sizeOfFreeHeap <= buffersizeForUninterruptable &&
		uninterruptableCounter <= 0)  {
#ifdef HEAPDEBUG
        cout << "Not enough free memory, only: " << sizeOfFreeHeap 
             << " bytes left." << endl
             << "Starting Garbage Collection" << endl;
#endif
		gc->Collect();

        //
        //reset allocation stats
        //
        numAlloc = 0;
        spcAlloc = 0;
	}
	
	void* result = NULL;
	FreeListEntry* current_entry = freeListStart;
	FreeListEntry* before_entry = NULL;

    //
	//find first fit
    //
	while (! ((current_entry->size == size) 
               || (current_entry->next == NULL) 
               || (current_entry->size >= (size + sizeof(FreeListEntry))))) { 
        before_entry = current_entry;
        current_entry = current_entry->next;
    }
	
    //
	// did we find a perfect fit?
	// if so, we simply remove this entry from the list
    //
    if (current_entry->size == size) {
        if (current_entry == freeListStart) { 
			// first one fitted - adjust the 'first-entry' pointer
            
            freeListStart = current_entry->next; 
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
        if (current_entry->size >= (size + sizeof(FreeListEntry))) {
            // save data from found entry
            int old_entry_size = current_entry->size;
            FreeListEntry* old_next = current_entry->next;
            
            result = current_entry;
            // create new entry and assign data
            FreeListEntry* replace_entry =  
                            (FreeListEntry*) ((int)current_entry + size);
            
            replace_entry->size = old_entry_size - size;
            replace_entry->next = old_next;
            if (current_entry == freeListStart) {
                freeListStart = replace_entry;
            } else {
                before_entry->next = replace_entry;
            }
        }  else { 
			// no space was left
			// running the GC here will most certainly result in data loss!
//#ifdef HEAPDEBUG 
            cout << "Not enough heap! Data loss is possible" << endl
			          << "FREE-Size: " << sizeOfFreeHeap 
                      << ", uninterruptableCounter: " 
                      << uninterruptableCounter << endl;
//#endif
            
			gc->Collect();
            numAlloc = 0;
            spcAlloc = 0;

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
    cout << "available heap size before alloc: " << sizeOfFreeHeap << endl;
#endif
	// update the available size
    sizeOfFreeHeap -= size;

#ifdef HEAPDEBUG 
    cout << "available heap size after alloc: " << sizeOfFreeHeap << endl;
    cout << "heap-start: " << hex  << objectSpace << endl;
    cout << "allocated at address: " << hex << result << endl;
#endif
    return result;
}

void Heap::Free(void* ptr) {
    if ( ((int)ptr < (int) this->objectSpace) &&
        ((int)ptr > (int) this->objectSpace + this->objectSpaceSize)) {
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
