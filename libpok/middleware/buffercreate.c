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
extern char*           pok_buffers_names[POK_CONFIG_NB_BUFFERS];
pok_size_t             pok_buffers_data_index = 0;


pok_ret_t pok_buffer_create (char*                                 name, 
                             const pok_port_size_t                 size, 
                             const pok_port_size_t                 msg_size, 
                             const pok_queueing_discipline_t       discipline,
                             pok_buffer_id_t*                      id)
{
   uint8_t     n;
   pok_ret_t   ret;

   for (n=0 ; n < POK_CONFIG_NB_BUFFERS ; n++)
   {
      if (streq (name, pok_buffers_names[n]))
      {
         if (pok_buffers[n].ready == TRUE)
         {
            return POK_ERRNO_READY;
         }

         ret = pok_event_create (&pok_buffers[n].lock);

         if (ret != POK_ERRNO_OK)
         {
            return ret;
         }

         pok_buffers[n].index                = pok_buffers_data_index;
         pok_buffers[n].ready                = TRUE;
         pok_buffers[n].empty                = TRUE;
         pok_buffers[n].size                 = size;
         pok_buffers[n].msgsize              = msg_size;
         pok_buffers[n].waiting_processes    = 0;
         pok_buffers[n].off_e                = 0;
         pok_buffers[n].off_b                = 0;
         pok_buffers[n].discipline           = discipline;

         pok_buffers_data_index              = pok_buffers_data_index + size;

         *id = n;

         return POK_ERRNO_OK;
      }

   }

   return POK_ERRNO_EINVAL;
}

#endif /* POK_NEEDS_BUFFERS */
#endif /* POK_NEEDS_MIDDLEWARE */
