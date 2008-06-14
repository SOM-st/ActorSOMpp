
#include "ExtendedVector.h"
#include <iterator>

template<class T>
int extended_vector<T>::index_of(T needle) {
	for (vector<T>::iterator it = begin(); it!=end(); ++it) {
		if (*it == needle) return distance(begin(), it);
		//cout << *it << endl;
	}
	return -1;
}