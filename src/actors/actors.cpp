/*
 *  actors.c
 *  CSOM
 *
 *  Created by Stefan Marr on 26/05/09.
 *  Copyright 2009 Vrije Universiteit Brussel. All rights reserved.
 *
 *  REM: in my implementation of the ilib, almost every thing is identified by
 *       the rank only, so no special mapping facilities necessary so far
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#include "../misc/ExtendedList.h"
#include "../vmobjects/ObjectFormats.h"
#include "../vmobjects/VMPointer.h"

// local data for faster access
int      _rank;
uint64_t _rank_mask;

// co-routine support
ExtendedList<pVMObject> activations;  // list of VMFrames representing co-routine activations

// constant/magic values
static int main_rank = 0;
static ilibRawReceivePort receive_port = 0;



#define FOR_ALL_RANKS(rank) \
  for (size_t rank = 0; rank < NUMBER_OF_ACTORS; ++rank)

#define FOR_ALL_OTHER_RANKS(rank) \
  FOR_ALL_RANKS(rank) if (rank != _rank)

void _go_parallel(char** argv) {
  ilibProcParam params;
  memset(&params, 0, sizeof(params));
  params.num_procs = NUMBER_OF_ACTORS;
  params.binary_name = NULL;
  params.argv = argv;
  
  params.tiles.x = params.tiles.y = 0;
  
#ifdef TILERA
#warning Fix this for Tile64
  params.tiles.width  = width;
  params.tiles.height = height;
#endif
  
  int err = ilib_proc_exec(1, &params);
  // TODO: check for error, but actually
  //       the following code should never be reached
  abort();
}

void actors_start(int argc, char** argv) {  
  int group_size = ilib_group_size(ILIB_GROUP_SIBLINGS);
  _rank       = ilib_group_rank(ILIB_GROUP_SIBLINGS);
  _rank_mask  = 1LL << uint64_t(_rank);
  
  if (group_size == 1 && group_size < NUMBER_OF_ACTORS) {
    _go_parallel(argv);
  }
  else {
    printf("MyRank: %d MyPID: %d Status: %s\n", _rank, getpid(), 
           (_rank == main_rank) ? "leader" : "follower");
    
    //sleep(20); // useful for debugging
  }
}

void _init_sink() {
  ilibSink sink;
  ilib_rawchan_start_sink(ILIB_GROUP_SIBLINGS, _rank, _rank, &sink);
  
  FOR_ALL_OTHER_RANKS(sender_rank) {
    ilib_rawchan_add_sink_sender(ILIB_GROUP_SIBLINGS, sender_rank, _rank, &sink);
  }
  ilib_rawchan_finish_sink(&sink);
}

void _open_channels() {
  FOR_ALL_RANKS(rank) {
    if (rank == _rank) {
      ilib_rawchan_open_receiver(rank, receive_port);
    }
    else {
      ilib_rawchan_open_sender(rank, rank);  
    }
  }
}


void actors_init_channels() {
  _init_sink();
  _open_channels();
}

actor_id_t actors_rank() {
  return _rank;
}
