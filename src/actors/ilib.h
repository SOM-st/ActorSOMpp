#ifndef ILIB_H_
#define ILIB_H_

#include "actors.h"
#include "synced_queue.h"

#include <stdint.h>

typedef int get_cycle_count_t;

inline int dummy_get_cycle_count() { return 0; }

inline int64_t get_cycle_count() {
  long long result;
  asm volatile(".byte 15;.byte 49" : "=A" (result));
  return result;
}

void ilib_init();

typedef unsigned long long int u_int64;
typedef unsigned int uint32_t;

typedef int ilibStatus;
typedef int ilibSink;
typedef int ilibBufChanSendPort;
typedef int ilibBufChanReceivePort;
typedef int ilibRequest;
typedef int ilibGroup;
typedef int ilibRawReceivePort;
typedef int ilibRawSendPort;

static const ilibGroup ILIB_GROUP_SIBLINGS = 0;

inline int ilib_msg_broadcast(int, int, void*, int, void*) { return 0; }
inline char* ilib_debug_strerror(int) {return (char*)"";}
void ilib_abort();


int ilib_proc_remaining();

/**
 * Definition: a group the set of interpreter instances, which constitute this VM
 */
int ilib_group_size(int);

/**
 * From MPI, the rank is a unique id for a process, thus, this
 * group rank, is a unique identifier for a member of this process group
 * REM: is currently never used properly, does not need to be implemented.
 */
int ilib_group_rank(int);

int spr_x_coordinate();
int spr_y_coordinate();

struct ilibProcParam {
  int num_procs;
  char* binary_name;
  char** argv;
  struct {
    int x, y, width, height;
  } tiles;
};

int ilib_proc_exec(unsigned int num_params, const ilibProcParam param_array[]);
void ilib_die(char*);
void tilera_exit();
inline int abort_if_error(char*, int e) { return e; }

# define ILIB_RAW_RECEIVE_PORT(x, y) enum {x = y};
# define ILIB_RAW_SEND_PORT(x, y) enum {x = y};
# define ILIB_STR_RECEIVE_PORT(x, y) enum {x = y};
# define ILIB_STR_SEND_PORT(x, y) enum {x = y};

inline int ilib_msg_barrier(int) { return 0; }

/**
 * Start the process of connecting a sink channel,
 * a channel that connects multiple senders to a
 * single receiver.
 */
int ilib_rawchan_start_sink(ilibGroup recv_group, int recv_rank, int recv_tag, ilibSink* sink_out);

/**
 * Add a sender to a sink channel started with ilib_rawchan_start_sink().
 */
inline int ilib_rawchan_add_sink_sender(ilibGroup send_group, int send_rank, int send_tag, ilibSink* sink) { return -1; }
inline int ilib_rawchan_finish_sink(int*) { return 0; }
int ilib_rawchan_open_receiver(int tag, ilibRawReceivePort port);
int ilib_rawchan_open_sender(int tag, ilibRawSendPort port);
void ilib_rawchan_send_1(ilibRawSendPort port, int32_t data);
void ilib_rawchan_send_2(ilibRawSendPort port, int32_t n0, int32_t n1);
void ilib_rawchan_send_3(ilibRawSendPort port, int32_t n0, int32_t n1, int32_t n2);
void ilib_rawchan_send_4(ilibRawSendPort port, int32_t n0, int32_t n1, int32_t n2, int32_t n3);
void ilib_rawchan_send_5(ilibRawSendPort port, int32_t n0, int32_t n1, int32_t n2, int32_t n3, int32_t n4);
void ilib_rawchan_send_6(ilibRawSendPort port, int32_t n0, int32_t n1, int32_t n2, int32_t n3, int32_t n4, int32_t n5);
void ilib_rawchan_send_7(ilibRawSendPort port, int32_t n0, int32_t n1, int32_t n2, int32_t n3, int32_t n4, int32_t n5, int32_t n6);
void ilib_rawchan_send_8(ilibRawSendPort port, int32_t n0, int32_t n1, int32_t n2, int32_t n3, int32_t n4, int32_t n5, int32_t n6, int32_t n7);
void ilib_rawchan_send_9(ilibRawSendPort port, int32_t n0, int32_t n1, int32_t n2, int32_t n3, int32_t n4, int32_t n5, int32_t n6, int32_t n7, int32_t n8);
void ilib_rawchan_send_uint64(ilibRawSendPort port, u_int64 data);
void ilib_rawchan_send_buffer(ilibRawSendPort port, const void* buffer, size_t size);

int32_t ilib_rawchan_receive(ilibRawReceivePort port);
u_int64 ilib_rawchan_receive_uint64(ilibRawReceivePort port);
void    ilib_rawchan_receive_buffer(ilibRawReceivePort port, void* buffer, size_t size);

int ilib_rawchan_available(ilibRawReceivePort port);

//inline int   ilib_msg_send(int, int, int, void*, int) { return 0; }
//inline int   ilib_msg_receive(int, int, int, void*, int, void*)  { return 0; }

//inline int ilib_bufchan_connect(ilibGroup group, int send_rank, int send_tag, int recv_rank, int recv_tag)  {
//  // just like a NOP, nothing to do here for a TileSqueak ilib, this implementation bound to TileSqueak
//  return 0;
//}
//
//inline int ilib_bufchan_open_sender(int tag, ilibBufChanSendPort* port) {
//  // just like a NOP, nothing to do here for a TileSqueak ilib, this implementation bound to TileSqueak
//  *port = tag;
//  return 0;
//}
//
//inline int ilib_bufchan_start_open_sender(int tag, ilibBufChanSendPort* port, ilibRequest* request_out) {
//  // just like a NOP, nothing to do here for a TileSqueak ilib, this implementation bound to TileSqueak
//  *port = tag;
//  *request_out = tag;
//  return 0;
//}

//int ilib_bufchan_open_receiver(int tag, void* buffer, size_t buffer_size, ilibBufChanReceivePort* port);
//
//inline int ilib_bufchan_calc_buffer_size(int num_packets, size_t packet_size) {
//  // just like a NOP, nothing to do here for a TileSqueak ilib, this implementation bound to TileSqueak
//  return 1;
//}

//int   ilib_bufchan_send(ilibBufChanSendPort port, const void* data, size_t size);
//void* ilib_bufchan_receive(ilibBufChanReceivePort port, size_t* size);
//void* ilib_bufchan_receive_noblock(ilibBufChanReceivePort port, size_t* size);

//inline void  ilib_bufchan_release_one(int) {}
//inline int   ilib_strchan_connect(int, int, int, int, int) { return 0; }
//inline int   ilib_strchan_open_sender(int, int) { return 0; }
//inline int   ilib_strchan_open_receiver(int, int) { return 0; }
//inline int   ilib_strchan_send_10(int, ...) { return 0; }
//inline void  ilib_strchan_receive_10(int, ...) {}
//inline int   ilib_rawchan_connect(int, int, int, int, int) { return 0; }
//inline int   ilib_rawchan_send_10(int, ...) {
//               return 0; }
//
//inline int  ilib_mem_get_cacheline_size() { return 64; }
inline void ilib_mem_invalidate(void*, int) {}
inline void ilib_mem_flush(void*, int) {}
inline void ilib_mem_fence() {}

inline int  ilib_wait(ilibRequest*, ilibStatus*) { return 0; }

struct ilibHeap {};
# define ILIB_MEM_HUGETLB 0
# define ILIB_MEM_SHARED 0
# define ILIB_MEM_INCOHERENT 0
# define ILIB_MEM_USER_MANAGED 0
# define MAP_CACHE_INCOHERENT 0
//inline int ilib_mem_create_heap(int, ilibHeap*) { return 0;}
//inline void* ilib_mem_malloc_heap(ilibHeap, int s) { return malloc(s); }

void* ilib_mem_memalign_heap(ilibHeap, int al, int sz);

typedef int ilibMutex;
inline void ilib_mutex_init(ilibMutex*) {}
inline int ilib_mutex_lock(ilibMutex*) {return 0;}
inline int ilib_mutex_trylock(ilibMutex*) {return 0;}
inline int ilib_mutex_unlock(ilibMutex*) {return 0;}

//inline void* memalign(int align, int sz) { return (void*) ( (int(malloc(sz + align)) + align - 1) & ~(align-1) ); }

#endif /*RVM_ILIB_H_*/
