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
 *                                      Copyright (c) 2007-2025 POK team
 */

#ifndef __POK_SPINLOCK_H__
#define __POK_SPINLOCK_H__

#include <core/multiprocessing.h>

typedef unsigned char pok_spinlock_t;

extern int spinlocks[POK_CONFIG_NB_PROCESSORS];

#define SPIN_UNLOCK(_spin_)                                                    \
  do {                                                                         \
    assert(_spin_);                                                            \
    assert(spinlocks[pok_get_proc_id()]);                                      \
    spinlocks[pok_get_proc_id()]--;                                            \
    (_spin_) = 0;                                                              \
  } while (0)

#define SPIN_LOCK(_spin_)                                                      \
  do {                                                                         \
    asm volatile("mov $1, %%al           \n\t"                                 \
                 "1:                     \n\t"                                 \
                 "lock xchg %0, %%al     \n\t"                                 \
                 "test %%al, %%al        \n\t"                                 \
                 "jnz 1b                 \n\t"                                 \
                 :                                                             \
                 : "m"(_spin_)                                                 \
                 : "%al");                                                     \
    spinlocks[pok_get_proc_id()]++;                                            \
  } while (0)

#define IS_LOCK(_spin_) (_spin_ == 1)

#endif /* !__POK_SPINLOCK_H__ */
