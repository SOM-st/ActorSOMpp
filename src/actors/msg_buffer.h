/*
 *  msg_buffer.h
 *  SOM
 *
 *  Created by Stefan Marr on 02/06/09.
 *  Copyright 2009 Vrije Universiteit Brussel. All rights reserved.
 *
 */

typedef uint32_t buffer_id_t;

bool msgbuffer_holds_data();
int32_t msgbuffer_read_atom();
void* msgbuffer_read_msg();
void msgbuffer_send_atom(buffer_id_t buffer_id, int32_t value);
void msgbuffer_send_msg(buffer_id_t buffer_id, int32_t value);
