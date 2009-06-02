/*
 *  synced_queue.h
 *  DataStructures
 *
 *  Created by Stefan Marr on 05/05/09.
 *  Copyright 2009 Vrije Universiteit Brussel. All rights reserved.
 *
 *  Implemented with a fixed size ring buffer, simple locking used to support multiple writers
 *  Derived from Ring_Buffer
 *  by 1997-06-19, Jarno Elonen <elonen@iki.fi>
 *  http://elonen.iki.fi/code/misc-notes/ringbuffer/
 *
 * REMARK:
 *   should look into performance penatly by pthreads mutex for writing
 *   might be there are look free strategies which perform better
 *   with a resonable sized buffer, add-and-inc, compare-and-swap
 *   might provide good preformance result for an asm supported implementation
 */

#ifndef SYNCED_QUEUE_H_
#define SYNCED_QUEUE_H_

#include <stdint.h>
#include <pthread.h>

typedef struct syncedqueue {
	int32_t* buffer;
  int32_t* buffer_end;

	int32_t* store;
  int32_t* read;

	size_t size;
  size_t used_buffer;

  pthread_mutex_t lock;
  bool initialized;
} syncedqueue, *p_syncedqueue;

void syncedqueue_initialize(p_syncedqueue sq, int32_t* const buffer, size_t size);
void syncedqueue_enqueue(p_syncedqueue sq, int32_t* const data, const size_t size);
void syncedqueue_enqueue_ext(p_syncedqueue sq, int32_t* const data, const size_t size);
void syncedqueue_enqueue_ex_with_header_valuet(p_syncedqueue sq, int32_t header, int32_t* const data, const size_t size);
void syncedqueue_dequeue(p_syncedqueue sq, int32_t* const data, const size_t size);
bool syncedqueue_is_initialized(p_syncedqueue sq);
bool syncedqueue_is_empty(p_syncedqueue sq);

#endif
