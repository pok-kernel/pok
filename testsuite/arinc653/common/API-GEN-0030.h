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
 * Created by julien on Mon May  4 18:09:52 2009 
 */

#ifndef __API_GEN_0020_H__
#define __API_GEN_0020_H__

#include <arinc653/types.h>

#define CHECK_RETURN_CODE_TYPE(arg) \
      if ((arg != NO_ERROR) && \
          (arg != NO_ACTION) && \
          (arg != NOT_AVAILABLE) && \
          (arg != INVALID_PARAM) && \
          (arg != INVALID_CONFIG) && \
          (arg != INVALID_MODE) && \
          (arg != TIMED_OUT)) return 0 else return 1;

#define CHECK_OPERATING_MODE_TYPE(arg) \
      if ((arg != IDLE) && (arg != COLD_START) && (arg != WARM_START) && (arg !=NORMAL)) return 0 else return 1;

#endif

