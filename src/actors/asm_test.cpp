/*
 *  asm_test.cpp
 *  sysctl
 *
 *  Created by Stefan Marr on 08/05/09.
 *  Copyright 2009 Vrije Universiteit Brussel. All rights reserved.
 *
 */

#include <stdint.h>
#include <err.h>

#include "asm.h"
#include "asm_test.h"

void test_XADD() {
  int32_t value = 4545;
  XADD(&value, 66);

  if (value != 4545 + 66) {
    warnx("XADD: value was not set correctly with a proper addition");
  }

  value = 45;
  XADD(&value, -66);

  if (value != 45 - 66) {
    warnx("XADD: value was not set correctly with a proper addition");
  }
}

void test_CMPXCHG() {
  int32_t value = 456;

  int32_t result = CMPXCHG(&value, 456, 111);

  if (result != 456) {
    warnx("CMPXCHG: returned result should be equal to old value.");
  }

  if (value != 111) {
    warnx("CMPXCHG: value was not changed, even when old value was equal to expected value;");
  }

  value = 333;
  result = CMPXCHG(&value, 222, 111);

  if (result != 333) {
    warnx("CMPXCHG: returned result should be equal to old value.");
  }

  if (value != 333) {
    warnx("CMPXCHG: value should not have been changed if expected value is not correct");
  }
}
