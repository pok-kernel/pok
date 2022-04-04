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

#ifdef POK_NEEDS_ARINC653_PROCESS

#include <arinc653/arincutils.h>
#include <libc/string.h>

/**
 * Simple function to check if the process exist
 * process_name => Name of the checked process
 */
uint32_t process_name_exist(PROCESS_NAME_TYPE *process_name) {
  int i;

  for (i = 0; i < POK_CONFIG_NB_THREADS; i++)
    if (!strcmp(*process_name, arinc_process_attribute[i].NAME))
      return i;
  return 0;
}

#endif
