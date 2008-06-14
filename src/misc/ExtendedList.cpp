
#include "ExtendedList.h"
#include <iterator>

template<class T>
int extended_list<T>::index_of(T needle) {
	for (list<T>::iterator it = begin(); it!=end(); ++it) {
		if (*it == needle) return distance(v.begin(), it);
		//cout << *it << endl;
	}
	return -1;
}