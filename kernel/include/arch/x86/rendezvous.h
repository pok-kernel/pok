/*
 *                               POK header
 *
 * The following file is a part of the POK project. Any modification should
 * be made according to the POK licence. You CANNOT use this file or a part
 * of a file for your own project.
 *
 * For more information on the POK licence, please see our LICENCE FILE
 *
 * Please follow the coding guidelines described in doc/CODING_GUIDELINES
 *
 *                                      Copyright (c) 2007-2024 POK team
 */

#ifndef __POK_RENDEZVOUS_H__
#define __POK_RENDEZVOUS_H__

#include <assert.h>

// A rendez-vous is implemented using a volatile unsigned integer.
// 0: the rendez-vous is not (or no longer) active
// 1: the rendez-vous has been started by the initiating core
// n: n-1 additional cores have joined the rendez-vous and are waiting
//    for it to become non-active

typedef volatile unsigned int *rendezvous_t;
typedef volatile unsigned int rendezvous;

//
// For the initiating core
//

// Start a new rendez-vous process
static inline void start_rendezvous(rendezvous_t r) {
  assert(!*r);
  *r = 1;
}

// Wait for other cores to join the rendez-vous
static inline void spin_wait_for_rendezvous(rendezvous_t r,
                                            unsigned int others_count) {
  assert(*r > 0);
  while (*r != others_count)
    ;
}

// Resolve the rendez-vous by making it non-active and unblock the other cores
static inline void unblock_rendezvous(rendezvous_t r) {
  assert(*r > 0);
  *r = 0;
}

//
// For the other cores
//

static inline void join_rendezvous(rendezvous_t r) {
  assert(*r > 0);
  asm volatile("lock incw %0" : "+m"(*r));
  while (*r)
    ;
}

#endif // __POK_RENDEZVOUS_H__
