#pragma once
#ifndef EXTENDEDVECTOR_H_
#define EXTENDEDVECTOR_H_
/* Not used because it's not working. Some people say stl containers are not supposed to be
   derived from. So maybe that's why it's not working....*/
#include <vector>
template <class T>
class extended_vector : public std::vector<T> {
public:
	extended_vector() : vector<T>() {};
	int index_of(T needle);
};

#endif
