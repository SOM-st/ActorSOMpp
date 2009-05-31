/*
 *  asm.h
 *  rvm
 *
 *  Created by Stefan Marr on 08/05/09.
 *  Copyright 2009 Vrije Universiteit Brussel. All rights reserved.
 *
 */


/**
 * XADD performs an atomic addtion on VAR by adding INCREMENT
 */
inline void XADD(int32_t* var, int32_t increment) {
  __asm__ __volatile__(
                       "lock xaddl %1,%0 ;\n"
                       : "=m"  (*var)
                       : "ir"  (increment), "m" (var)
                       //:                                 /* empty clobber-list */
                       );
}

/**
 * Compares the value of DEST with EXPECTED_VAL
 * iff they are equal, DEST is set to NEW_VAL
 * return is alsways the olf value of DEST
 */
inline int32_t CMPXCHG(int32_t* dest, int32_t expected_val, int32_t new_val) {
  int32_t found_value = 0;
  __asm__ __volatile__(
                       "lock cmpxchg %2,%0 ;\n"
                       : "+m" (*dest), "=a" (found_value) /* out */
                       : "ir"  (new_val), "a" (expected_val) /* in */
                       //:                                 /* empty clobber-list */
                       );
  return found_value;
}
