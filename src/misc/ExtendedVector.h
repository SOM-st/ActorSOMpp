#pragma once
#ifndef EXTENDEDVECTOR_H_

#include <vector>
#include <iterator>

template <class T>
class ExtendedVector{
public:
	ExtendedVector();
	void Add(const T& ptr);
	void AddIfAbsent(const T& ptr);
	void Clear();
	int Size();
	int IndexOf(const T& needle);
	T Get(int index);

	T operator[](int key) 
	{
		return theVector[key];
	}
    
    typedef typename vector<T>::iterator iterator_t;
    typedef typename vector<T>::const_iterator const_iterator_t;
    iterator_t begin() // changed return type to above typedefs
    {
        return theVector.begin();
    }
    // implement for const objects...
    const_iterator_t begin() const { return theVector.begin(); }

private:
	vector<T> theVector;
	
};


template <class T>
ExtendedVector<T>::ExtendedVector()
{
	theVector.clear();
}

template <class T>
void ExtendedVector<T>::Add(const T& ptr)
{
	theVector.push_back(ptr);
}

template <class T>
void ExtendedVector<T>::AddIfAbsent(const T& ptr)
{
	if (IndexOf(ptr) == -1) Add(ptr);
}

template <class T>
T ExtendedVector<T>::Get(int index)
{
	return theVector.at(index);
}

template <class T>
int ExtendedVector<T>::Size()
{
	return theVector.size();
}

template <class T>
int ExtendedVector<T>::IndexOf(const T& needle) {
	for (iterator_t it = theVector.begin(); it!=theVector.end(); ++it) {
		if (*it == needle) return distance(theVector.begin(), it);
		//cout << *it << endl;
	}
	return -1;
}

#endif
