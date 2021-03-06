/*
 *  ObjectTable.cpp
 *  SOM
 *
 *  Created by Daniel Hefenbrock on 5/2/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ObjectTable.h"

#include "../vmobjects/VMObject.h"


ObjectTable* ObjectTable::theObjectTable = NULL;


VMObject* ObjectTable::operator[](Index index) const {
    if (index.value >= size) {
        std::cout << "[ObjectTable] Accessing invalid entry " << index.value << " (size: " 
                    << size << " max capacity: " << objectTable.max_size() << ")" << std::endl;
    }
    // todo: check for free entry
    return objectTable[index.value].content.object;
}

/*bool ObjectTable::IsLocal(Index index) const {
    return (objectTable[index.value].actor == actors_id()
            || objectTable[index.value].actor == ACTOR_OMNI);
}*/

/*bool ObjectTable::IsRemote(Index index) const {
    return (objectTable[index.value].actor != actors_id()
            && objectTable[index.value].actor != ACTOR_OMNI);
}*/


ObjectTable::Index ObjectTable::AddObject(VMObject* object) {
    
    // debug check
/*    if (contains(object)) {
               std::cout << "[ObjectTable] WARNING duplicate object " << object << std::endl;
    }  */
    
    Index index;
    index.is_iint = false;
    // check if we can re-use an existing entry
    if (0 != free_stack.value) {
        index.value = free_stack.value;
        // pop free stack
        free_stack.value = objectTable[index.value].content.index.value;
        // add object
        objectTable[index.value].content.object = object;
    } else {
        // extend the object table
        objectTable.push_back(Entry(object));
        index.value = size++;
    }
    
    if (NULL != object) {
        object->SetObjectTableIndex(index);
    }
   
//    std::cout << "[ObjectTable] added " << object << " " << (NULL==object ? " NULL" : typeid(*object).name()) << " at index "<< size << std::endl;
    return index;
}

// Add remote object to table
/* ObjectTable::Index ObjectTable::AddRemoteObject(GlobalObjectId id) {
// should remote objects become part of this object table?
    // its rather in efficient to find them in the table, and a point object is created anyway
    Index idx;
    return idx;
} */
 
// Mark object as omnipresent
/* void ObjectTable::MarkOmnipresent(Index index) {
#warning not implemented
}*/

void ObjectTable::RemoveObject(Index index) {
    if (index.value >= size) {
        std::cout << "Accessing invalid ObjectTable entry" << std::endl;
    }
//    std::cout << "[ObjectTable] removed object at index "<< index << std::endl;
    
    // push entry to free stack
    objectTable[index.value].content.index.value = free_stack.value;
    free_stack = index;
}


bool ObjectTable::Contains(VMObject* object) {
    if (NULL == object) {
        return false;
    }
    for (int i=0; i<size; ++i) {
        if (object == objectTable[i].content.object) {
            return true;
        }
    }
    return false;
}

