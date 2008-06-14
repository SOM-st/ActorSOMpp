#pragma once
#ifndef EXTENDEDLIST_H_
#define EXTENDEDLIST_H_
/* Not used because it's not working. Some people say stl containers are not supposed to be
   derived from. So maybe that's why it's not working....*/
#include <list>
#include <memory>
template <class T>
class extended_list : public std::list<T> {
public:
	extended_list() : list<T>() {};
	int index_of(T needle);
};

#endif
