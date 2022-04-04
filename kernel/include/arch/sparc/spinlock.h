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

/*
 * FIXME: MUST BE IMPLEMENTED
 */

typedef unsigned int pok_spinlock_t;

#define SPIN_UNLOCK(_spin_)                                                    \
  do {                                                                         \
    assert(_spin_);                                                            \
    (_spin_) = 0;                                                              \
  } while (0)

#define SPIN_LOCK(_spin_)                                                      \
  do {                                                                         \
    asm volatile("1:              \n"                                          \
                 "ldstub [%0], %1 \n"                                          \
                 "tst %1          \n"                                          \
                 "bnz 1b          \n"                                          \
                 : /* no output */                                             \
                 : "r"(&(_spin_)), "r"(1));                                    \
  } while (0)

#endif /* !__POK_SPINLOCK_H__ */
