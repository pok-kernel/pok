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

#include <arinc653/partition.h>
#include <libc/stdio.h>

void test() {
  OPERATING_MODE_TYPE var;
  RETURN_CODE_TYPE ret;
  PARTITION_STATUS_TYPE status GET_PARTITION_STATUS(&status, &ret);
  if (CHECK_OPERATING_MODE_TYPE(status.OPERATING_MODE) != 1) {
    printf("ERROR\n");
  }
}
