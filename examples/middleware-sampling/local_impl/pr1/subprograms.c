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

#include "types.h"

void user_do_ping_spg(simple_type *data_source);

/*****************/
/* do_ping_spg   */
/*****************/

void do_ping_spg(simple_type *data_source) { user_send(data_source); }
