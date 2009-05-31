/*
 *  actors.h
 *  SOM
 *
 *  Created by Stefan Marr on 31/05/09.
 *  Copyright 2009 Vrije Universiteit Brussel. All rights reserved.
 *
 */

#include <stdint.h>

#include "ilib.h"

#define NUMBER_OF_ACTORS 2

typedef uint8_t actor_id_t;

void actors_start(int argc, char** argv);
void actors_init_channels();
actor_id_t actors_rank();
