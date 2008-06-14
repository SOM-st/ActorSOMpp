#pragma once
#ifndef GENERATIONCONTEXTCORE_H_
#define GENERATIONCONTEXTCORE_H_
#include <vector>
#include <string>
#include "../misc/defs.h"
using namespace std;
class generation_context_core {
protected:
	static int index_of(pList haystack, pString needle) {
		pList::iterator i = haystack.end();
		for (pList::iterator it = haystack.begin(); it!=i; ++it) {
			if (*it == needle) return distance(haystack.begin(), it);
			//cout << *it << endl;
		}
		return -1;
	
	}
};

#endif