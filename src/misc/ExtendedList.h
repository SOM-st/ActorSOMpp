#pragma once
#ifndef EXTENDEDLIST_H_
#define EXTENDEDLIST_H_

#include <list>
#include <memory>
#include <iterator>
#include <iostream>

template <class T>
class ExtendedList {
public:
	explicit ExtendedList();
	//~ExtendedList();
	
	void Add(const T& ptr);
	void AddIfAbsent(const T& ptr);
	void AddAll(const ExtendedList<T>* list);
    void PushBack(const T& ptr);
	void Clear();
	int Size() const;
	T Get(int index);
	int IndexOf(const T& needle);

    typedef typename std::list<T>::iterator iterator_t;
    typedef typename std::list<T>::const_iterator const_iterator_t;
    
    iterator_t Begin() {
        return theList.begin();
    }
    // implement for const objects...
    const_iterator_t Begin() const { return theList.begin(); }

private:
	std::list<T> theList;
};


template <class T>
ExtendedList<T>::ExtendedList() {
	theList.clear();
}


template <class T>
void ExtendedList<T>::Add(const T& ptr) {
	theList.push_back(ptr);
}


template <class T>
void ExtendedList<T>::AddAll(const ExtendedList<T>* list) {
	theList.merge(list->theList);
}


template <class T>
void ExtendedList<T>::AddIfAbsent(const T& ptr) {
	if (IndexOf(ptr) == -1) Add(ptr);
}


template <class T>
void ExtendedList<T>::Clear() {
	theList.clear();
}


template <class T>
T ExtendedList<T>::Get(int index) {
    for (iterator_t it = theList.begin(); it!=theList.end(); ++it){
		if (index == 0) return *it;
		--index;
	}
	return NULL;
}


template <class T>
int ExtendedList<T>::Size() const {
	return theList.size();
}


template <class T>
int ExtendedList<T>::IndexOf(const T& needle) {
    for (iterator_t it = theList.begin(); it!=theList.end(); ++it) {
		if (*it == needle) 
            return distance(theList.begin(), it);
		//cout << *it << endl;
	}
	return -1;
}


template < class T >
void ExtendedList<T>::PushBack( const T& ptr ) {
    theList.push_back(ptr);
}


#endif
