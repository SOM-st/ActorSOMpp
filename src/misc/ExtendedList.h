#pragma once
#ifndef EXTENDEDLIST_H_
#define EXTENDEDLIST_H_

#include <list>
#include <memory>
#include <iterator>

template <class T>
class ExtendedList{
public:
	explicit ExtendedList();
	//~ExtendedList();
	
	void Add(const T& ptr);
	void AddIfAbsent(const T& ptr);
	void AddAll(const ExtendedList<T>* list);
    void push_back(const T& ptr);
	void Clear();
	int Size();
	T get(int index);
	int IndexOf(const T& needle);

private:
	std::list<T> theList;
};


template <class T>
ExtendedList<T>::ExtendedList()
{
	theList.clear();
}

//template <class T>
//ExtendedList<T>::~ExtendedList()
//{
//}


template <class T>
void ExtendedList<T>::Add(const T& ptr)
{
	theList.push_back(ptr);
}

template <class T>
void ExtendedList<T>::AddAll(const ExtendedList<T>* list)
{
	theList.merge(list->theList);
}

template <class T>
void ExtendedList<T>::AddIfAbsent(const T& ptr)
{
	if (IndexOf(ptr) == -1) Add(ptr);
}

template <class T>
void ExtendedList<T>::Clear()
{
	theList.clear();
}

template <class T>
T ExtendedList<T>::get(int index)
{
    for (std::list<T>::iterator it = theList.begin(); it!=theList.end(); ++it){
		if (index == 0) return *it;
		--index;
	}
	return NULL;
}

template <class T>
int ExtendedList<T>::Size()
{
	return theList.size();
}

template <class T>
int ExtendedList<T>::IndexOf(const T& needle) {
    for (std::list<T>::iterator it = theList.begin(); it!=theList.end(); ++it) {
		if (*it == needle) 
            return distance(theList.begin(), it);
		//cout << *it << endl;
	}
	return -1;
}

template < class T >
void ExtendedList<T>::push_back( const T& ptr )
{
    theList.push_back(ptr);
}

#endif
