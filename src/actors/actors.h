/*
 *  actors.h
 *  SOM
 *
 *  Created by Stefan Marr on 31/05/09.
 *  Copyright 2009 Vrije Universiteit Brussel. All rights reserved.
 *
 */

#ifndef _ACTORS_H_
#define _ACTORS_H_

#include <stdint.h>
#include <stddef.h>

#define NUMBER_OF_ACTORS 2

// this actor id represents all omnipresent objects, usually globals which are
// imposed by the VM
#define ACTOR_OMNI 0x7F

typedef uint8_t actor_id_t;

void actors_init();
void actors_start(int argc, char** argv);
void actors_shutdown();

actor_id_t actors_id();
bool actors_is_main_actor();
bool actors_is_local(actor_id_t id);
bool actors_is_remote(actor_id_t id);


bool    actors_msgbuffer_holds_data();
int32_t actors_msgbuffer_read_atom();
void    actors_msgbuffer_read_msg(void** buffer, size_t* size);

void    actors_msgbuffer_send_atom(actor_id_t actor_id, int32_t value);
void    actors_msgbuffer_send_msg(actor_id_t actor_id, void* msg_buffer, size_t size);

#endif
