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

#include <libc/stdio.h>

void user_thread_starting() {
  printf("[PART2] Partition initialisation\n");
  printf("In this tests, different errors would be raised :\n");
  printf("At value 02 : divide by zero\n");
  printf("At value 08 : memory violation\n");
  printf("At value 12 : the task raises an application error\n");
  printf("At value 16 : call infinitly a recursive function \n");
  printf("At value 18 : the task performs an invalid syscall\n");
}
