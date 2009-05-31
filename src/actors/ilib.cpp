/*
 *  ilib.c
 *  rvm
 *
 *  Created by Stefan Marr on 03/05/09.
 *  Copyright 2009 Vrije Universiteit Brussel. All rights reserved.
 *
 *  This is a clean room implementation of the ilib interfaces as they are
 *  utiized by the rvm/TileSqueak.
 *  It is meant to provide the same set of routines on a typical Intel
 *  shared memory architecture.
 */

#include "ilib.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdbool.h>
#include <errno.h>
#include <err.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define GLOBAL_SHARED_MEM_ID "/ilib-global-v003"
#define CHANNEL_BUFFER_SIZE 96

typedef struct lib_raw_channel {
    syncedqueue queue;
    int32_t buffer[CHANNEL_BUFFER_SIZE];
} lib_raw_channel;

typedef struct lib_globals {
  pid_t   owner_pid;
  size_t  number_of_processes_in_group;
  size_t  number_of_running_processes;
  int32_t last_rank;

  lib_raw_channel raw_channel[NUMBER_OF_ACTORS];
  //MultipleWriterPtrQueue buffered_queue[Number_Of_Interpreters];

  pthread_mutex_t global_lock;
} lib_globals, *p_lib_globals;

typedef struct proc_locals {
  int32_t rank;
  pid_t   parent_pid;
  pid_t   pid;

  size_t receive_port_channel_no[3];  // indexed by ilibRawReceivePort
} proc_locals, *p_proc_locals;

// pointer to process shared memory with lib global information
p_lib_globals lib_glob = NULL;

// process local information
proc_locals proc_info;

void _unit() {
    pthread_mutex_lock(&lib_glob->global_lock);

    lib_glob->number_of_running_processes--;
    bool last_process = (0 == lib_glob->number_of_running_processes);

    pthread_mutex_unlock(&lib_glob->global_lock);

    if (last_process) {
        pthread_mutex_destroy(&lib_glob->global_lock);
    }

    munmap(lib_glob, sizeof(lib_globals));

    if (last_process) {
        shm_unlink(GLOBAL_SHARED_MEM_ID);
    }
}

void _init_global_data_structures() {
    memset(lib_glob, 0, sizeof(lib_globals));
    lib_glob->last_rank = 0;
    lib_glob->number_of_running_processes = 1;
    lib_glob->number_of_processes_in_group = 1;

    lib_glob->owner_pid = getpid();

    pthread_mutexattr_t global_lock_attr;
    pthread_mutexattr_init(&global_lock_attr);
    pthread_mutexattr_setpshared(&global_lock_attr, PTHREAD_PROCESS_SHARED);

    if (0 != pthread_mutex_init(&lib_glob->global_lock, &global_lock_attr)) {
        // error
        // TODO: do it properly
        perror("Error in _init_global_data_structures");
    }
}

/**
 * This function makes sure that each rank of the process group is running on
 * a dedicated processing unit (PU) reported by the OS.
 * It does not mean a physical unit, but includes also logical units like hyper
 * threads.
 */
void _init_processor_affinity() {
#ifdef __APPLE__
  // NOP
  // Mac OS X does not support setting explicit affinity to a PU
  // It only supports expressing cache affinity
  // and this is only for one process i.e. threads in a process
  // http://developer.apple.com/ReleaseNotes/Performance/RN-AffinityAPI/index.html
#else
  // On Linux, it looks pretty easy:
  // http://www.linuxjournal.com/article/6799
  // http://www.ibm.com/developerworks/linux/library/l-affinity.html
  int rank = ilib_group_rank(0);
  cpu_set_t affinity_mask;
  CPU_ZERO(&affinity_mask);
  CPU_SET(rank, &affinity_mask);
  
  if (sched_setaffinity(getpid(), sizeof(affinity_mask), &affinity_mask) < 0) {
    perror("Failed to set affinity");
    abort();
  }
  
#endif
  
  sleep(0); // make sure the OS schedule has a chance to do as we told him
}

void _reinit_global_data_structures() {
    warnx("Some confusion with global data, which is shared system wide. Currently there is no support for more then one instance of a program using this library!");
    pthread_mutex_destroy(&lib_glob->global_lock);
    _init_global_data_structures();
}

void _init_process_local_data() {
    proc_info.parent_pid = getppid();
    proc_info.pid = getpid();

    proc_info.receive_port_channel_no[0] = -1;
    proc_info.receive_port_channel_no[1] = -1;
    proc_info.receive_port_channel_no[2] = -1;

    if (lib_glob->owner_pid != proc_info.parent_pid
        && lib_glob->owner_pid != proc_info.pid) {
        _reinit_global_data_structures();
    }

    pthread_mutex_lock(&lib_glob->global_lock);

    if (proc_info.pid == lib_glob->owner_pid) {
        proc_info.rank = 0;
    } else {
        lib_glob->last_rank++;
        proc_info.rank = lib_glob->last_rank;

        lib_glob->number_of_running_processes++;
    }

    pthread_mutex_unlock(&lib_glob->global_lock);

    atexit(_unit);  // register clean up hook
}

void ilib_init() {
  bool initialize_globaly = false;

  int lib_sh_fd = shm_open(GLOBAL_SHARED_MEM_ID,
                           O_RDWR, S_IRUSR | S_IWUSR);
  if (lib_sh_fd < 0) {
    switch (errno) {
      case ENOENT:
        initialize_globaly = true;
        lib_sh_fd = shm_open(GLOBAL_SHARED_MEM_ID,
                             O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
        if (lib_sh_fd < 0) {
          // error
          // TODO: do it properly
          perror("Error in init on shm_open with O_CREAT");
        }

        if (-1 == ftruncate(lib_sh_fd, sizeof(lib_globals))) {
          // error
          // TODO: do it properly
          perror("Error in init on ftruncate");
        }
        break;
      default:
        // error
        // TODO: do it properly
        perror("Error in init on shm_open");
        break;
    }
  }

  if (lib_sh_fd >= 0) {
    lib_glob = (p_lib_globals)mmap(NULL, sizeof(lib_globals),
                                   PROT_READ | PROT_WRITE, MAP_SHARED, lib_sh_fd, 0);
    if (MAP_FAILED == lib_glob) {
      perror("Error in init on mmap");
    }
  }

  // init if necessary
  if (initialize_globaly) {
    _init_global_data_structures();
  }

  _init_process_local_data();
  _init_processor_affinity();
}

int ilib_proc_remaining() {
    if (!lib_glob)
        return -1;

    return ilib_group_size(ILIB_GROUP_SIBLINGS) - lib_glob->number_of_running_processes;
}

int ilib_group_size(int) {
    if (!lib_glob)
        return -1;

    return lib_glob->number_of_processes_in_group;
    //return Number_Of_Cores * Number_Of_HyperThreads_Per_Core * Number_Of_Chips;
}

int ilib_group_rank(int) {
    return proc_info.rank;
}

// REM: for intel cpus, we are impliying a chain/row of cores, should be good enough for now
// is also implied in the special case of CPU_Core::set_width_height
int spr_x_coordinate() { return ilib_group_rank(ILIB_GROUP_SIBLINGS); }
int spr_y_coordinate() { return 0; }

int ilib_proc_exec(unsigned int num_params, const ilibProcParam param_array[]) {
    for (size_t p = 0; p < num_params; p++) {
        lib_glob->number_of_processes_in_group = param_array[p].num_procs;

        for (size_t i = 1; i < param_array[p].num_procs; i++) {
            pid_t pid = fork(); //or vfork for performance

            if (0 == pid) {
                // child
                execv(param_array[p].argv[0], param_array[p].argv);
            } else if (pid > 0) {
                // master
            } else {
                // error
                // TODO: do it properly
                perror("Error in ilib_proc_exec");
            }
        }

        execv(param_array[p].argv[0], param_array[p].argv);
    }
    return -1;
}

/** RAW CHANNEL SUPPORT **/

int32_t ilib_rawchan_receive(ilibRawReceivePort port) {
  int32_t result;
  size_t recv_tag = proc_info.receive_port_channel_no[port];
  syncedqueue_dequeue(&lib_glob->raw_channel[recv_tag].queue, &result, 1);
  return result;
}

u_int64 ilib_rawchan_receive_uint64(ilibRawReceivePort port) {
  u_int64_t result;
  size_t recv_tag = proc_info.receive_port_channel_no[port];
  syncedqueue_dequeue(&lib_glob->raw_channel[recv_tag].queue, (int32_t*)&result, 2);
  return result;
}

/**
 * WARNING: this is currently only for 32bit systems
 */
void ilib_rawchan_receive_buffer(ilibRawReceivePort port, void* buffer, size_t size) {
  size_t buffer_item_cnt = ceil((double)size / (double)sizeof(int32_t));
  size_t recv_tag = proc_info.receive_port_channel_no[port];
  p_syncedqueue queue = &lib_glob->raw_channel[recv_tag].queue;

  printf("ilib_rawchan_receive_buffer(%d, %p, %d) on queue: %p\n", recv_tag, buffer, buffer_item_cnt, queue);
  syncedqueue_dequeue(queue, (int32_t*)buffer, buffer_item_cnt);
}

int ilib_rawchan_start_sink(ilibGroup recv_group, int recv_rank, int recv_tag, ilibSink* sink_out) {
  syncedqueue_initialize(&lib_glob->raw_channel[recv_tag].queue, lib_glob->raw_channel[recv_tag].buffer, CHANNEL_BUFFER_SIZE);

  *sink_out = recv_tag;
  return 0;
}

int ilib_rawchan_open_receiver(int tag, ilibRawReceivePort port) {
  if (port < 0 || port > 3) {
    warnx("This implementation is not compatable with ilibRawReceivePort numbers other then 0..2, there are only 3 sinks posible on Tile64.");
    abort();
  }
  
  while (!syncedqueue_is_initialized(&lib_glob->raw_channel[port].queue)) {
    usleep(100);
  }

  proc_info.receive_port_channel_no[port] = tag;
  return 0;
}

int ilib_rawchan_open_sender(int tag, ilibRawSendPort port) {
  while (!syncedqueue_is_initialized(&lib_glob->raw_channel[port].queue)) {
    usleep(100);
  }  
}


void ilib_rawchan_send_1(ilibRawSendPort port, int32_t data) {
  syncedqueue_enqueue(&lib_glob->raw_channel[port].queue, &data, 1);
}

void ilib_rawchan_send_2(ilibRawSendPort port, int32_t n0, int32_t n1) {
  int32_t data[] = {n0, n1};
  syncedqueue_enqueue(&lib_glob->raw_channel[port].queue, data, 2);
}

void ilib_rawchan_send_3(ilibRawSendPort port, int32_t n0, int32_t n1, int32_t n2) {
  int32_t data[] = {n0, n1, n2};
  syncedqueue_enqueue(&lib_glob->raw_channel[port].queue, data, 3);
}

void ilib_rawchan_send_4(ilibRawSendPort port, int32_t n0, int32_t n1, int32_t n2, int32_t n3) {
  int32_t data[] = {n0, n1, n2, n3};
  syncedqueue_enqueue(&lib_glob->raw_channel[port].queue, data, 4);
}

void ilib_rawchan_send_5(ilibRawSendPort port, int32_t n0, int32_t n1, int32_t n2, int32_t n3, int32_t n4) {
  int32_t data[] = {n0, n1, n2, n3, n4};
  syncedqueue_enqueue(&lib_glob->raw_channel[port].queue, data, 5);
}

void ilib_rawchan_send_6(ilibRawSendPort port, int32_t n0, int32_t n1, int32_t n2, int32_t n3, int32_t n4, int32_t n5) {
  int32_t data[] = {n0, n1, n2, n3, n4, n5};
  syncedqueue_enqueue(&lib_glob->raw_channel[port].queue, data, 6);
}

void ilib_rawchan_send_7(ilibRawSendPort port, int32_t n0, int32_t n1, int32_t n2, int32_t n3, int32_t n4, int32_t n5, int32_t n6) {
  int32_t data[] = {n0, n1, n2, n3, n4, n5, n6};
  syncedqueue_enqueue(&lib_glob->raw_channel[port].queue, data, 7);
}

void ilib_rawchan_send_8(ilibRawSendPort port, int32_t n0, int32_t n1, int32_t n2, int32_t n3, int32_t n4, int32_t n5, int32_t n6, int32_t n7) {
  int32_t data[] = {n0, n1, n2, n3, n4, n5, n6, n7};
  syncedqueue_enqueue(&lib_glob->raw_channel[port].queue, data, 8);
}

void ilib_rawchan_send_9(ilibRawSendPort port, int32_t n0, int32_t n1, int32_t n2, int32_t n3, int32_t n4, int32_t n5, int32_t n6, int32_t n7, int32_t n8) {
  int32_t data[] = {n0, n1, n2, n3, n4, n5, n6, n7, n8};
  syncedqueue_enqueue(&lib_glob->raw_channel[port].queue, data, 9);
}

void ilib_rawchan_send_uint64(ilibRawSendPort port, u_int64 data) {
  syncedqueue_enqueue(&lib_glob->raw_channel[port].queue, (int32_t*)&data, 2);
}

void ilib_rawchan_send_buffer(ilibRawSendPort port, const void* buffer, size_t size) {
  size_t buffer_item_cnt = ceil((double)size / (double)sizeof(int32_t));
  printf("ilib_rawchan_send_buffer(%d, %p, %d) on queue: %p\n", port, buffer, buffer_item_cnt, &lib_glob->raw_channel[port].queue);
  
  syncedqueue_enqueue_ext(&lib_glob->raw_channel[port].queue, (int32_t*)buffer, buffer_item_cnt);
}

int ilib_rawchan_available(ilibRawReceivePort port) {
  return syncedqueue_is_empty(&lib_glob->raw_channel[port].queue);
}


/** BUFFERED CHANNEL SUPPORT */

//int ilib_bufchan_send(ilibBufChanSendPort port, const void* data, size_t size) {
//  lib_glob->buffered_queue[port].enqueue(data);
//}
//
//void* ilib_bufchan_receive(ilibBufChanReceivePort port, size_t* size) {
//  while (lib_glob->buffered_queue[port].empty()) {
//    usleep(10);
//  }
//
//  return (void*)lib_glob->buffered_queue[port].dequeue();
//}
//
//void* ilib_bufchan_receive_noblock(ilibBufChanReceivePort port, size_t* size) {
//  *size = -1;
//  if (lib_glob->buffered_queue[port].empty()) {
//    return NULL;
//  } else {
//    return (void*)lib_glob->buffered_queue[port].dequeue();
//  }
//}
//
//int ilib_bufchan_open_receiver(int tag, void* buffer, size_t buffer_size, ilibBufChanReceivePort* port) {
//  // just like a NOP, nothing to do here for a TileSqueak ilib, this implementation bound to TileSqueak
//  *port = tag;
//  free(buffer);
//  
//  lib_glob->buffered_queue[tag] = MultipleWriterPtrQueue();
//  return 0;
//}

// from header only moved here

void ilib_die(char* err_msg) {
    warnx(err_msg);
    abort();
}

//void ilib_abort() { abort(); }
//void* ilib_mem_memalign_heap(ilibHeap, int al, int sz) { return memalign(al, sz); }
