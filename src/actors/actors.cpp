/*
 *  actors.cpp
 *  CSOM
 *
 *  Created by Stefan Marr on 26/05/09.
 *  Copyright 2009 Vrije Universiteit Brussel. All rights reserved.
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/mman.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>

#include "../misc/ExtendedList.h"
#include "../vmobjects/ObjectFormats.h"
#include "../vmobjects/VMPointer.h"
#include "synced_queue.h"
#include "messages.h"
#include "ActorMessaging.h"

#define ACTOR_QUEUE_BUFFER_SIZE 256

typedef struct actor_globals {
    pid_t       owner_pid;
    actor_id_t  last_actor_id;
    pthread_mutex_t global_lock;
    syncedqueue queues[NUMBER_OF_ACTORS];
    int32_t     queue_buffer[NUMBER_OF_ACTORS][ACTOR_QUEUE_BUFFER_SIZE];
} actor_globals, *p_actor_globals;

// local data for faster access
actor_id_t  _local_id;
bool        _main_actor;
pid_t       _parent_pid;
pid_t       _pid;

pid_t       _actor_processes[NUMBER_OF_ACTORS];

// pointer to global information
p_actor_globals _globals = NULL;

// constant/magic values
static actor_id_t _main_id = 0;


#define SHARED_MEM_ID "/actor_comm_space_I"

void _uninit() {
    if (_main_actor) {
        pthread_mutex_destroy(&_globals->global_lock);
    }

    munmap(_globals, sizeof(actor_globals));

    if (_main_actor) {
        shm_unlink(SHARED_MEM_ID);
    }
}

void _unit_main() {
    pthread_mutex_destroy(&_globals->global_lock);
    munmap(_globals, sizeof(actor_globals));
    shm_unlink(SHARED_MEM_ID);
}

void _init_global_data_structures() {
    memset(_globals, 0, sizeof(actor_globals));
    _globals->last_actor_id = 0;
    _globals->owner_pid = getpid();

    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);

    if (0 != pthread_mutex_init(&_globals->global_lock, &attr)) {
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

    cpu_set_t affinity_mask;
    CPU_ZERO(&affinity_mask);
    CPU_SET(_local_id, &affinity_mask);

    if (sched_setaffinity(getpid(), sizeof(affinity_mask), &affinity_mask) < 0) {
        perror("Failed to set affinity");
        abort();
    }

#endif

    sleep(0); // make sure the OS schedule has a chance to do as we told him
}

void _reinit_global_data_structures() {
    warnx("Some confusion with global data, which is shared system wide. "
          "Currently there is no support for more then one instance of a "
          "program using this library!");
    pthread_mutex_destroy(&_globals->global_lock);
    _init_global_data_structures();
}

void _init_process_local_data() {
    _parent_pid = getppid();
    _pid        = getpid();

    if (_globals->owner_pid != _parent_pid
        && _globals->owner_pid != _pid) {
        _reinit_global_data_structures();
    }

    pthread_mutex_lock(&_globals->global_lock);

    if (_pid == _globals->owner_pid) {
        _local_id = 0;
        _main_actor = true;
    } else {
        _main_actor = false;
        _globals->last_actor_id++;
        _local_id = _globals->last_actor_id;
    }

    pthread_mutex_unlock(&_globals->global_lock);

    atexit(_uninit);  // register clean up hook
}

void actors_init() {
    bool initialize_globaly = false;

    int fd = shm_open(SHARED_MEM_ID, O_RDWR, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        switch (errno) {
            case ENOENT:
                initialize_globaly = true;
                fd = shm_open(SHARED_MEM_ID, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
                if (fd < 0) {
                    // error
                    // TODO: do it properly
                    perror("Error in init on shm_open with O_CREAT");
                }

                if (-1 == ftruncate(fd, sizeof(actor_globals))) {
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

    if (fd >= 0) {
        _globals = (p_actor_globals)mmap(NULL, sizeof(actor_globals),
                                    PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (MAP_FAILED == _globals) {
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

void _go_parallel(char** argv) {
    _actor_processes[0] = _pid;
    
    for (size_t i = 1; i < NUMBER_OF_ACTORS; i++) {
        pid_t pid = fork();

        if (0 == pid) {
            // child
            execv(argv[0], argv);
        } else if (pid > 0) {
            // master
            _actor_processes[i] = pid;
        } else {
            // error
            // TODO: do it properly
            perror("Error in _go_parallel");
        }
    }
}

void _init_communication() {
    for (actor_id_t id = 0; id < NUMBER_OF_ACTORS; id++) {
        syncedqueue_initialize(&_globals->queues[id], _globals->queue_buffer[id], ACTOR_QUEUE_BUFFER_SIZE);
    }
}


void actors_start(int argc, char** argv) {
    if (_main_actor) {
        _init_communication();
        _go_parallel(argv);
    }
    else {
        printf("MyActorId: %d MyPID: %d Status: %s\n", _local_id, getpid(),
               (_main_actor) ? "leader" : "follower");

    //sleep(20); // useful for debugging
  }
}

void actors_shutdown() {
    // send message to force shut down
    ExitMsg msg;
    for (size_t i = 1; i < NUMBER_OF_ACTORS; i++) {
        ActorMessaging::SendMessage(&msg, i);
    }
    
    // wait for childs
    for (size_t i = 1; i < NUMBER_OF_ACTORS; i++) {
        waitpid(_actor_processes[i], NULL, 0);
    }
}

actor_id_t actors_id() {
    return _local_id;
}

bool actors_is_main_actor() {
    return _local_id == 0;
}

bool actors_is_local(actor_id_t id) { return id == _local_id; }
bool actors_is_remote(actor_id_t id) { return id != _local_id && id != ACTOR_OMNI; }


bool actors_msgbuffer_holds_data() {
    return !syncedqueue_is_empty(&_globals->queues[_local_id]);
}

int32_t actors_msgbuffer_read_atom() {
    int32_t data;
    syncedqueue_dequeue(&_globals->queues[_local_id], &data, 1);
    return data;
}

void actors_msgbuffer_read_msg(void** buffer, size_t* size) {
    int32_t msg_length; // msg length in bytes, but queue calculates in int32_t's
    syncedqueue_dequeue(&_globals->queues[_local_id], &msg_length, 1);

    int32_t real_msg_length = ceil((double)msg_length / (double) sizeof(int32_t));

    *buffer = malloc(real_msg_length * sizeof(int32_t)); // to avoid overflow

    syncedqueue_dequeue(&_globals->queues[_local_id], (int32_t*)*buffer, real_msg_length);

    *size = msg_length;
}

void actors_msgbuffer_send_atom(actor_id_t actor_id, int32_t value) {
    syncedqueue_enqueue(&_globals->queues[actor_id], &value, 1);
}

void actors_msgbuffer_send_msg(actor_id_t actor_id, void* msg_buffer, size_t size) {
    int32_t real_msg_length = ceil((double)size / (double) sizeof(int32_t));

    syncedqueue_enqueue_ex_with_header_valuet(&_globals->queues[actor_id], size, (int32_t*)msg_buffer, real_msg_length);
}


