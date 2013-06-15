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
 * Created by julien on Thu Jan 15 23:34:13 2009 
 */

#include <core/dependencies.h>

#ifdef POK_NEEDS_MIDDLEWARE
#ifdef POK_NEEDS_BUFFERS

#include <errno.h>
#include <types.h>
#include <core/time.h>
#include <core/event.h>
#include <libc/string.h>
#include <middleware/buffer.h>

extern pok_buffer_t    pok_buffers[POK_CONFIG_NB_BUFFERS];
extern char            pok_buffers_data[1024];

pok_ret_t pok_buffer_receive (const pok_buffer_id_t                id, 
                              const uint64_t                       timeout, 
                              void*                                data, 
                              pok_port_size_t*                     len)
{
   pok_ret_t ret;

   if (id > POK_CONFIG_NB_BUFFERS)
   {
      return POK_ERRNO_EINVAL;
   }

   if (pok_buffers[id].ready == FALSE)
   {
      return POK_ERRNO_EINVAL;
   }

   if (data == NULL)
   {
      return POK_ERRNO_EINVAL;
   }

   pok_event_lock (pok_buffers[id].lock);

   while (pok_buffers[id].empty == TRUE)
   {
      if (timeout == 0)
      {
         pok_event_unlock (pok_buffers[id].lock);
      }
      else
      {
         ret = pok_event_wait (pok_buffers[id].lock, timeout);
         if (ret != POK_ERRNO_OK)
         {
            pok_event_unlock (pok_buffers[id].lock);
            return ret;
         }
      }
   }

   memcpy (data, &pok_buffers_data[pok_buffers[id].index + pok_buffers[id].off_b], pok_buffers[id].msgsize);
   pok_buffers[id].off_b = (pok_buffers[id].off_b + pok_buffers[id].msgsize) % pok_buffers[id].size;
   if (pok_buffers[id].off_b == pok_buffers[id].off_e)
   {
      pok_buffers[id].empty = TRUE;
   }

   pok_buffers[id].full = FALSE;

   *len = pok_buffers[id].msgsize;

   pok_event_unlock (pok_buffers[id].lock);

   pok_event_broadcast (pok_buffers[id].lock);

   return POK_ERRNO_OK;
}

#endif /* POK_NEEDS_BUFFERS */
#endif
