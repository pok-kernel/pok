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

#include <arinc653/types.h>

int test() {
  RETURN_CODE_TYPE var;
  var = NO_ERROR;
  var = NO_ACTION;
  var = NOT_AVAILABLE;
  var = INVALID_PARAM;
  var = INVALID_CONFIG;
  var = INVALID_MODE;
  var = TIMED_OUT;
}
