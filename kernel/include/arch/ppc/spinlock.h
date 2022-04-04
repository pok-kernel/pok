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
 *                                      Copyright (c) 2007-2022 POK team
 */

#ifndef __POK_SPINLOCK_H__
#define __POK_SPINLOCK_H__

#include <assert.h>

typedef unsigned int pok_spinlock_t;

#define SPIN_UNLOCK(_spin_)                                                    \
  do {                                                                         \
    assert(_spin_);                                                            \
    (_spin_) = 0;                                                              \
  } while (0)

#define SPIN_LOCK(_spin_)                                                      \
  do {                                                                         \
    unsigned int val;                                                          \
    asm volatile("\n"                                                          \
                 "1:\n\t"                                                      \
                 "lwarx    %0,0,%1       \n\t"                                 \
                 "cmpwi    %0,0          \n\t"                                 \
                 "bne      1b            \n\t"                                 \
                 "stwcx.   %2,0,%1       \n\t"                                 \
                 "bne      1b            \n\t"                                 \
                 : "=&r"(val)                                                  \
                 : "r"(&_spin_), "r"(1));                                      \
  } while (0)

#endif /* !__POK_SPINLOCK_H__ */
