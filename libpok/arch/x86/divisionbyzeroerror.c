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

#include <arch.h>

void pok_division_by_zero_error() {
  asm volatile("mov $0,%%ebx  \n\t"
               "div %%ebx     \n\t"
               :
               :
               : "%eax", "%ebx", "%edx");
  __builtin_unreachable();
}
