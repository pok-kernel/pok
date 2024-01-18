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
  PARTITION_STATUS_TYPE status;
  RETURN_CODE_TYPE ret;
  GET_PARTITION_STATUS(&status, &ret);

  if (status.PERIOD != 1000000000) {
    printf("ERROR PERIOD\n");
  }

  if (status.DURATION != 1000000000) {
    printf("ERROR DURATION\n");
  }

  if (status.IDENTIFIER != 0) {
    printf("ERROR IDENTIFIER\n");
  }

  if (status.LOCK_LEVEL != 0) {
    printf("ERROR LOCK LEVEL\n");
  }

  if (status.OPERATING_MODE != NORMAL) {
    printf("ERROR OPERATING MODE\n");
  }

  if (ret != NO_ERROR) {
    printf("ERROR RETURN CODE\n");
  }

  printf("ALL TESTS WERE PASSED\n");
}
