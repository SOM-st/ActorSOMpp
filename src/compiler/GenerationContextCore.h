#pragma once
#ifndef GENERATIONCONTEXTCORE_H_
#define GENERATIONCONTEXTCORE_H_
#include <vector>
#include <string>
#include "../misc/defs.h"
using namespace std;
class generation_context_core {
protected:
    /*static int index_of(const pList& haystack, const VMObject* needle) {
        pList::const_iterator i = haystack.end();
		for (pList::const_iterator it = haystack.begin(); it!=i; ++it) {
			if (*it == needle) return distance(haystack.begin(), it);
			//cout << *it << endl;
		}
		return -1;
	
	}

    static int index_of2(const list<pString>& haystack, const pString& needle) {
		list<pString> ::const_iterator i = haystack.end();
		for (list<pString> ::const_iterator it = haystack.begin(); it!=i; ++it) {
			if (*it == needle) return distance(haystack.begin(), it);
			//cout << *it << endl;
		}
		return -1;
	
	}*/
};

#endif