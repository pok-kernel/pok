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
 *                                      Copyright (c) 2007-2021 POK team
 */

#ifndef __LIBPOK_ARCH_H__

#include <types.h>

/**
 * This function is called when a division by zero occurs in with 64-bit
 * numbers. This function will divide by 0 to force the processor to raise a
 * Division by 0 error.
 */
__attribute__((noreturn)) void pok_division_by_zero_error(void);

void pok_arch_idle(void);

#endif /* __LIBPOK_ARCH_H__ */
