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

#ifdef POK_NEEDS_PORTS_QUEUEING

#include <types.h>
#include <errno.h>

#include <core/lockobj.h>
#include <core/sched.h>
#include <core/time.h>

#include <middleware/port.h>
#include <middleware/queue.h>

extern pok_port_t    pok_ports[POK_CONFIG_NB_PORTS];
extern pok_queue_t   pok_queue;


pok_ret_t pok_port_queueing_send (const pok_port_id_t   id, 
                                  const void*           data, 
                                  const pok_port_size_t len, 
                                  uint64_t              timeout)
{
   pok_lockobj_lockattr_t  lockattr;
   (void) lockattr;

   pok_ret_t ret;

   if (id > POK_CONFIG_NB_PORTS)
   {
      return POK_ERRNO_EINVAL;
   }

   if (len <= 0)
   {
      return POK_ERRNO_SIZE;
   }

   if (data == NULL)
   {
      return POK_ERRNO_EINVAL;
   }

   if (! pok_own_port (POK_SCHED_CURRENT_PARTITION, id))
   {
      return POK_ERRNO_PORT;
   }

   if (pok_ports[id].ready != TRUE)
   {
      return POK_ERRNO_PORT;
   }

   if (len > pok_ports[id].size)
   {
      return POK_ERRNO_SIZE;
   }

   if (pok_ports[id].direction != POK_PORT_DIRECTION_OUT)
   {
      return POK_ERRNO_DIRECTION;
   }

   if (pok_ports[id].partition != POK_SCHED_CURRENT_PARTITION)
   {
      return POK_ERRNO_EPERM;
   }

   if (pok_ports[id].kind != POK_PORT_KIND_QUEUEING)
   {
      return POK_ERRNO_KIND;
   }

   ret = pok_lockobj_lock (&pok_ports[id].lock, NULL);

   if (ret != POK_ERRNO_OK)
   {
      return ret;
   }

   if (timeout != 0)
   {
      timeout = timeout + POK_GETTICK();
   }

   while (len > pok_port_available_size (id))
   {
      if (timeout == 0)
      {
         pok_lockobj_unlock (&pok_ports[id].lock, NULL);
         return POK_ERRNO_FULL;
      }
      else
      {
         ret = pok_lockobj_eventwait (&pok_ports[id].lock, timeout);
         if (ret != POK_ERRNO_OK)
         {
            pok_lockobj_unlock (&pok_ports[id].lock, NULL);
            return (ret);
         }
      }
   }

   pok_port_write (id, data, len);

   pok_ports[id].must_be_flushed = TRUE;

   ret = pok_lockobj_unlock (&pok_ports[id].lock, NULL);

   if (ret != POK_ERRNO_OK)
   {
      return ret;
   }
   return POK_ERRNO_OK;
}

#endif

