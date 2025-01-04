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

#ifdef POK_NEEDS_MIDDLEWARE
#ifdef POK_NEEDS_BUFFERS

#include <errno.h>
#include <libc/string.h>
#include <middleware/buffer.h>
#include <types.h>

extern pok_buffer_t pok_buffers[POK_CONFIG_NB_BUFFERS];

pok_ret_t pok_buffer_status(const pok_buffer_id_t id,
                            pok_buffer_status_t *status) {
  if (id > POK_CONFIG_NB_BUFFERS) {
    return POK_ERRNO_EINVAL;
  }

  /* FIXME : should fix this later */
  status->nb_messages = 0;
  status->max_messages = pok_buffers[id].size / pok_buffers[id].msgsize;
  status->message_size = pok_buffers[id].msgsize;
  status->waiting_processes = 0;

  return POK_ERRNO_OK;
}

#endif /* POK_NEEDS_BUFFERS */
#endif
