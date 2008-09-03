//
//#include "ExtendedVector.h"
//#include <iterator>
//
//template <class T>
//ExtendedVector<T>::ExtendedVector()
//{
//	theVector.clear();
//}
//
//template <class T>
//void ExtendedVector<T>::Add(VMObject *ptr)
//{
//	theVector.push_back(ptr);
//}
//
//template <class T>
//void ExtendedVector<T>::AddIfAbsent(VMObject *ptr)
//{
//	if (IndexOf(ptr) == -1) Add(ptr);
//}
//
//template <class T>
//VMObject* ExtendedVector<T>::Get(int index)
//{
//	return theVector.at(index);
//}
//
//template <class T>
//int ExtendedVector<T>::Size()
//{
//	return theVector.size();
//}
//
//template <class T>
//int ExtendedVector<T>::IndexOf(VMObject *needle) {
//	for (vector<VMObject*>::iterator it = theVector.begin(); it!=theVector.end(); ++it) {
//		if (*it == needle) return distance(theVector.begin(), it);
//		//cout << *it << endl;
//	}
//	return -1;
//}