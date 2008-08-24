#pragma once

#ifndef FREELISTENTRY_H_
#define FREELISTENTRY_H_

class FreeListEntry
{
public:
	FreeListEntry(size_t s = 0);
	~FreeListEntry();

	FreeListEntry* GetNext();
	void SetNext(FreeListEntry* new_next);
private:
	FreeListEntry* next;
	size_t size;
};

#endif