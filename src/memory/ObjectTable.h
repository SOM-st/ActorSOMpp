#pragma once

/*
 *  ObjectTable.h
 *  SOM
 *
 *  Created by Daniel Hefenbrock on 5/2/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef OBJECTTABLE_H_
#define OBJECTTABLE_H_


#include <vector>
#include <iostream>

#include "../misc/defs.h"

#include "../actors/actors.h"

//macro to access the object table
#define _OBJECT_TABLE ObjectTable::GetObjectTable()

class VMObject;

class ObjectTable
{
public:
    // Most stuff is private here, only VMPointer can access it.
    template<class> friend class VMPointer;
    
    // We can only use 31 bits, which should be more than enough.
    typedef struct Index {
        unsigned long value: 31;
        bool is_iint: 1; // assumed to be zero
        
        inline bool operator<(Index o) const {
            return value < o.value;
        }
        
        /*inline bool operator==(Index o) const {
            return value == o.value && is_iint == o.is_iint;
        }*/
    } Index;
    
    // Singleton accessor
    static ObjectTable& GetObjectTable() {
        if (!theObjectTable) {
            theObjectTable = new ObjectTable();
        }
        return *theObjectTable;
    }
    
    // Self-explainatory.
    unsigned long GetSize() const {
        return size;
    }
    
private:
    
    // This is how an object table entry looks. Later we will
    // add the reference count here.
    typedef struct Entry {
        Entry(VMObject* object) {
            content.object = object;
        }        
        union {
            VMObject* object;
            Index     index;
        } content;
    } Entry;
    
    // The singleton instance.
    static ObjectTable* theObjectTable;
    // The actual object table.
    std::vector<ObjectTable::Entry> objectTable;
    // Current size of the object table.
    unsigned long size;
    // Free entries stack, 0 = empty
    Index free_stack;
    
    //
    // Private constructor - Singleton
    //
    ObjectTable() : size(0) {
        free_stack.value = 0;
        // At index 0, we store a NULL pointer
        AddObject((VMObject*) NULL);
    }
    
    //
    // Manipulating
    //
    
    // Access entry at index
    VMObject* operator[](Index index) const;
    
    // Add object to table
    Index AddObject(VMObject* object);
    
    // Add remote object to table
    //Index AddRemoteObject(VMObject* object, actor_id_t actor);
    
    // Mark object as omnipresent
    //void MarkOmnipresent(Index index);
    //TODO figure out whether we need this, omnipresents sounds like useful optimization...
    
    // Remove object from table
    void RemoveObject(Index index);
    
    // Helper method for debugging.
    bool Contains(VMObject* object);
    
    // Check whether object is local or omnipresent
    //bool IsLocal(Index index) const;

    // Check whether object is remote
    //bool IsRemote(Index index) const;

};

#endif
