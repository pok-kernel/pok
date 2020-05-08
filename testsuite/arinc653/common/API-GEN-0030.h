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

#ifndef __API_GEN_0020_H__
#define __API_GEN_0020_H__

#include <arinc653/types.h>

#define CHECK_RETURN_CODE_TYPE(arg)                                            \
  if ((arg != NO_ERROR) && (arg != NO_ACTION) && (arg != NOT_AVAILABLE) &&     \
      (arg != INVALID_PARAM) && (arg != INVALID_CONFIG) &&                     \
      (arg != INVALID_MODE) && (arg != TIMED_OUT))                             \
    return 0 else return 1;

#define CHECK_OPERATING_MODE_TYPE(arg)                                         \
  if ((arg != IDLE) && (arg != COLD_START) && (arg != WARM_START) &&           \
      (arg != NORMAL))                                                         \
    return 0 else return 1;

#endif
