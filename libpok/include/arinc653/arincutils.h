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
 * Created by matias on Wed May 04 09:34:13 2012 
 */

#ifdef POK_NEEDS_ARINC653_PROCESS

#include <arinc653/process.h>

/**
 * Struct for save data
 * NAME => Not use by pok
 * BASE_PRIORITY => This value, in pok, is modified. Here we save the base value
 */
typedef struct {
  PROCESS_NAME_TYPE       NAME;
  PRIORITY_TYPE           BASE_PRIORITY;
  STACK_SIZE_TYPE         STACK_SIZE;
} ARINC_ATTRIBUTE;

ARINC_ATTRIBUTE              arinc_process_attribute[POK_CONFIG_NB_THREADS];

uint32_t process_name_exist (PROCESS_NAME_TYPE *process_name);

#endif
