#pragma once

#ifndef HEAPALLOCATOR_H_
#define HEAPALLOCATOR_H_

#include "../memory/Heap.h"
#include "../vm/Universe.h"

template <class T> class HeapAllocator
{
public:
  typedef T                 value_type;
  typedef value_type*       pointer;
  typedef const value_type* const_pointer;
  typedef value_type&       reference;
  typedef const value_type& const_reference;
  typedef std::size_t       size_type;
  typedef std::ptrdiff_t    difference_type;
  
  template <class U> 
  struct rebind { typedef HeapAllocator<U> other; };

  HeapAllocator() {}
  HeapAllocator(Heap* heap) 
  { 
	  this->heap = heap;
  }
  HeapAllocator(const HeapAllocator& org) 
  {
		this->heap = org.heap;
  }
  template <class U> 
  HeapAllocator(const HeapAllocator<U>& org) 
  {
	  this->heap = org.heap;
  }
  ~HeapAllocator() {}

  pointer address(reference x) const { return &x; }
  const_pointer address(const_reference x) const { 
    return x;
  }

  pointer allocate(size_type n, const_pointer = 0) {
	void* p = heap->Allocate(n * sizeof(value_type));
    if (!p)
      throw std::bad_alloc();
    return static_cast<pointer>(p);
  }

  void deallocate(pointer p, size_type) { 
	  heap->Free(p);
  }

  size_type max_size() const { 
    return static_cast<size_type>(-1) / sizeof(T);
  }

  void construct(pointer p, const value_type& x) { 
    new(p) value_type(x); 
  }
  void destroy(pointer p) { p->~value_type(); }

private:
  void operator=(const HeapAllocator&);
  Heap* heap;
};

template<> class HeapAllocator<void>
{
  typedef void        value_type;
  typedef void*       pointer;
  typedef const void* const_pointer;

  template <class U> 
  struct rebind { typedef HeapAllocator<U> other; };
  Heap* heap;
};


template <class T>
inline bool operator==(const HeapAllocator<T>&, 
                       const HeapAllocator<T>&) {
  return true;
}

template <class T>
inline bool operator!=(const HeapAllocator<T>&, 
                       const HeapAllocator<T>&) {
  return false;
}

#endif
