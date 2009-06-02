/*
 *  synced_queue.cpp
 *  DataStructures
 *
 *  Created by Stefan Marr on 05/05/09.
 *  Copyright 2009 Vrije Universiteit Brussel. All rights reserved.
 */

#include "synced_queue.h"
#include "asm.h"

#include <unistd.h>
#include <stdio.h>

void syncedqueue_initialize(p_syncedqueue sq, int32_t* const buffer, size_t size) {
  pthread_mutex_init(&sq->lock, NULL);
  sq->buffer = buffer;
  sq->buffer_end = sq->buffer + size;

	sq->store = sq->read = buffer;

  sq->used_buffer = 0;
  sq->size = size;

  sq->initialized = true;
}

inline size_t _store_max(p_syncedqueue sq) {
  return sq->size - sq->used_buffer;
}

void _store(p_syncedqueue sq, int32_t* const data, const size_t size) {
  printf("[%d] _store(%p, %p, %d)\n", getpid(), sq, data, size);
  while (size > _store_max(sq)) {
    usleep(5);
  }
  printf("[%d] _store after sleep\n", getpid());

  for (size_t i = 0; i < size; i++) {
    *sq->store = data[i];
    sq->store++;
    if (sq->store == sq->buffer_end) {
      sq->store = sq->buffer;
    }
  }

  XADD((int32_t*)&sq->used_buffer, size);
}

void syncedqueue_enqueue_ex_with_header_valuet(p_syncedqueue sq, int32_t header, int32_t* const data, const size_t size) {
  pthread_mutex_lock(&sq->lock);
  
  _store(sq, &header, 1);  // store header first
  
  size_t left = size;
  int32_t* values = data;
  
  while (left > 32) {
    _store(sq, values, 32);
    values += 32;
    left -= 32;
  }
  
  _store(sq, values, left);
  
  pthread_mutex_unlock(&sq->lock);
}

void syncedqueue_enqueue_ext(p_syncedqueue sq, int32_t* const data, const size_t size) {
  pthread_mutex_lock(&sq->lock);

  size_t left = size;
  int32_t* values = data;

  while (left > 32) {
    _store(sq, values, 32);
    values += 32;
    left -= 32;
  }

  _store(sq, values, left);

  pthread_mutex_unlock(&sq->lock);
}


void syncedqueue_enqueue(p_syncedqueue sq, int32_t* const data, const size_t size) {
  pthread_mutex_lock(&sq->lock);
    _store(sq, data, size);
  pthread_mutex_unlock(&sq->lock);
}

inline size_t _read_max(p_syncedqueue sq) {
  return sq->used_buffer;
}

void syncedqueue_dequeue(p_syncedqueue sq, int32_t* const data, const size_t size) {
  printf("[%d] syncedqueue_dequeue\n", getpid());

  size_t to_be_read = size;
  size_t do_read;

  while (to_be_read > 0) {
    do_read = _read_max(sq);
    if (do_read == 0) {
      usleep(5);
    } else {
      if (do_read > to_be_read) {
        do_read = to_be_read;
      }

      for (size_t i = 0; i < do_read; i++) {
        data[i] = *sq->read;
        sq->read++;
        if (sq->read == sq->buffer_end) {
          sq->read = sq->buffer;
        }
      }

      XADD((int32_t*)&sq->used_buffer, -do_read);
      to_be_read -= do_read;
    }
  }
}

bool syncedqueue_is_initialized(p_syncedqueue sq) {
  return sq->initialized;
}

bool syncedqueue_is_empty(p_syncedqueue sq) {
  return sq->used_buffer == 0;
}
