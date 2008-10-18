#include "Heap.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "../vmobjects/VMObject.h"

Heap::Heap(int object_space_size)// : globals(INT32_MAX)
{
    allocCount = 0;
	object_space = (void*) malloc(object_space_size);
	if (!object_space)
	{
		std::cout << "Failed to allocate the initial "<< object_space_size << " bytes for the GC. Panic.\n" << std::endl;
		exit(1);
	}
	memset(object_space, 0, object_space_size);
	size_of_free_heap = object_space_size;
	this->object_space_size = object_space_size;
	this->buffersize_for_uninterruptable = (int) (object_space_size * 0.1);
    uninterruptable_counter = 0;
	
	free_list_start = (free_list_entry*) object_space;
	free_list_start->size = object_space_size;
	free_list_start->next = NULL;
	gc = new GarbageCollector(this);
}

Heap::~Heap()
{
	free(object_space);
}

VMObject* Heap::AllocateObject(size_t size)
{
    VMObject* vmo = (VMObject*) Allocate(size);
    vmo->SetObjectSize(size);
    return vmo;
}

void* Heap::Allocate(size_t size)
{
    ++allocCount;
	if (size == 0) return NULL;
    if (size < sizeof(free_list_entry)) 
    {
        return internalAllocate(size);
    }
#ifdef HEAPDEBUG 
    std::cout << "allocating: " << (int)size << "bytes" << std::endl;
#endif
	if (size_of_free_heap <= buffersize_for_uninterruptable &&
		uninterruptable_counter <= 0) 
	{
#ifdef HEAPDEBUG
        cout << "Not enough free memory, only: " << size_of_free_heap << " bytes left." <<endl;
        cout << "Starting Garbage Collection" << endl;
#endif
		gc->Collect();
	}
	
	void* result = NULL;
	free_list_entry* current_entry = free_list_start;
	free_list_entry* before_entry = NULL;

	//find first fit
	while (! ((current_entry->size == size) 
               || (current_entry->next == NULL) 
               || (current_entry->size >= (size + sizeof(free_list_entry))))) { 
        before_entry = current_entry;
        current_entry = current_entry->next;
    }
	
	// did we find a perfect fit?
	// if so, we simply remove this entry from the list
    if (current_entry->size == size) {
        if (current_entry == free_list_start) { 
			// first one fitted - adjust the 'first-entry' pointer
            free_list_start = current_entry->next; 
			//PROBLEM (also in CSOM?): what if last possible allocate is a perfect fit?
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
            free_list_entry* replace_entry =  (free_list_entry*) ((int)current_entry + size);
            
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
            std::cout << "Not enough heap! Data loss is possible" << std::endl;
			std::cout << "FREE-Size: " << size_of_free_heap << ", uninterruptable_counter: " << uninterruptable_counter << endl;
//#endif
            
			gc->Collect();
            //fulfill initial request
            result = Allocate(size);
        }
    }
           
    if(!result) {
		std::cout << "Failed to allocate "<< (int)size <<" bytes. Panic." << std::endl;
        //Universe_exit(-1);
		exit(1);
    }
    memset(result, 0, size);

#ifdef HEAPDEBUG 
    std::cout << "available heap size before alloc: " << size_of_free_heap << std::endl;
#endif
	// update the available size
    size_of_free_heap -= size;

#ifdef HEAPDEBUG 
    std::cout << "available heap size after alloc: " << size_of_free_heap << std::endl;
    std::cout << "heap-start: " << hex  << object_space << std::endl;
    std::cout << "allocated at address: " << hex << result << std::endl;
#endif
    return result;
}

void Heap::Free(void* ptr)
{
    if ( ((int)ptr < (int) this->object_space) &&
        ((int)ptr > (int) this->object_space + this->object_space_size))
    {
        internalFree(ptr);
    }
}

void Heap::Free(void* /*ptr*/, int /*size*/)
{
    //add referenced space to free list
}

void Heap::internalFree(void* ptr)
{
    free(ptr);
}

void* Heap::internalAllocate(size_t size)
{
    if (size == 0) return NULL;
    void* result = malloc(size);
    if(!result)
    {
        cout << "Failed to allocate " << size << " Bytes." << endl;
        _UNIVERSE->Quit(-1);
    }
    memset(result, 0, size);
    return result;
}
