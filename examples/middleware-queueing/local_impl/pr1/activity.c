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
 *                                      Copyright (c) 2007-2025 POK team
 */

#include "types.h"
#include <errno.h>
#include <middleware/port.h>
#include <types.h>
uint8_t port_pinger_data_source_id;
void *pinger_job() {
  int ret;
  simple_type data_source_request_var;

  ret = pok_port_queueing_create("dataout", 8, POK_PORT_DIRECTION_OUT,
                                 POK_QUEUEING_DISCIPLINE_FIFO,
                                 &port_pinger_data_source_id);
  printf("Create queue, return=%d\n", ret);
  printf("port id=%d\n", port_pinger_data_source_id);

  while (1) {
    do_ping_spg(&(data_source_request_var));
    ret = pok_port_queueing_send(port_pinger_data_source_id,
                                 &data_source_request_var, 4, 0);
    if (ret != POK_ERRNO_OK) {
      printf("SEND ERROR, ret=%d\n", ret);
    }
    pok_thread_sleep(1000000);
  }
}
