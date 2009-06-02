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

#define NUMBER_OF_ACTORS 2

// this actor id represents all omnipresent objects, usually globals which are
// imposed by the VM
#define ACTOR_OMNI 0x7F

typedef uint8_t actor_id_t;

void actors_init();
void actors_start(int argc, char** argv);
void actors_init_communication();
actor_id_t actors_id();

#endif
