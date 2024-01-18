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

#include <gtypes.h>
#include <libc/stdio.h>
#include <types.h>

/***************************/
/* user_flight_director   */
/**************************/

uint32_t fd_iterations = 0;
int result;
int zero = 0;

void user_flight_director(types__pagerequestcmd request,
                          types__pagecontent *page) {
  printf("[FD] USER FLIGHT DIRECTOR\n");
  printf("[FD] EXECUTED %d times\n", fd_iterations);
  fd_iterations++;
  *page = fd_iterations;

  /*
  if (fd_iterations == 10)
  {
     result = fd_iterations / zero;
  }
  */
}
