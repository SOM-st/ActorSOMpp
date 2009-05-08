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
    if (index >= size) {
        std::cout << "[ObjectTable] Accessing invalid entry " << index << " (size: " 
                    << size << " max capacity: " << objectTable.max_size() << ")" << std::endl;
    }
    // todo: check for free entry
    return objectTable[index].object;
}


ObjectTable::Index ObjectTable::AddObject(VMObject* object) {
    objectTable.push_back(Entry(object));
    if (NULL != object) {
        object->SetObjectTableIndex(size);
    }
    if (contains(object)) {
        std::cout << "[ObjectTable] WARNING duplicate object " << object << std::endl;
    }
    std::cout << "[ObjectTable] added " << object << " " << (NULL==object ? " NULL" : typeid(*object).name()) << " at index "<< size << std::endl;
    return size++;
}


void ObjectTable::RemoveObject(Index index) {
    if (index >= size) {
        std::cout << "Accessing invalid ObjectTable entry" << std::endl;
    }
    std::cout << "[ObjectTabke] removed object at index "<< index << std::endl;
    objectTable[index].object = NULL;
    // todo: reuse entries. free list etc.
}


bool ObjectTable::contains(VMObject* object) {
    if (NULL == object) {
        return false;
    }
    for (int i=0; i<size; ++i) {
        if (object == objectTable[i].object) {
            return true;
        }
    }
    return false;
}

