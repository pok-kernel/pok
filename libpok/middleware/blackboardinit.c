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
 *                                      Copyright (c) 2007-2020 POK team
 */

#ifdef POK_NEEDS_MIDDLEWARE
#ifdef POK_NEEDS_BLACKBOARDS

#include <errno.h>
#include <middleware/blackboard.h>
#include <types.h>

extern pok_blackboard_t pok_blackboards[POK_CONFIG_NB_BLACKBOARDS];

pok_ret_t pok_blackboard_init(void) {
  uint8_t n;

  for (n = 0; n < POK_CONFIG_NB_BLACKBOARDS; n++) {
    pok_blackboards[n].ready = FALSE;
  }
  return POK_ERRNO_OK;
}

#endif
#endif
