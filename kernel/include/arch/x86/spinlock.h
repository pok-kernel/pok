/*
 *                               POK header
 * 
 * The following file is a part of the POK project. Any modification should
 * made according to the POK licence. You CANNOT use this file or a part of
 * this file is this part of a file for your own project
 *
 * For more information on the POK licence, please see our LICENCE FILE
 *
 * Please follow the coding guidelines described in doc/CODING_GUIDELINES
 *
 *                                      Copyright (c) 2007-2009 POK team 
 *
 * Created by julien on Thu Jan 15 23:34:13 2009 
 */


#ifndef __POK_SPINLOCK_H__
#define __POK_SPINLOCK_H__

typedef unsigned char pok_spinlock_t;

#define SPIN_UNLOCK(_spin_)                                     \
{                                                               \
  (_spin_) = 0;                                                 \
}

#define SPIN_LOCK(_spin_)                                       \
  asm volatile ("mov $1, %%al           \n\t"                   \
                "1:                     \n\t"                   \
                "lock xchg %0, %%al     \n\t"                   \
                "test %%al, %%al        \n\t"                   \
                "jnz 1b                 \n\t"                   \
                :                                               \
                : "m" (_spin_)                                  \
                : "%al")

#endif /* !__POK_SPINLOCK_H__ */
