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
#include <core/time.h>
#include <core/sched.h>

#include <middleware/port.h>
#include <middleware/queue.h>

#include <libc.h>

extern pok_port_t    pok_ports[POK_CONFIG_NB_PORTS];

pok_ret_t pok_port_queueing_receive (const pok_port_id_t    id, 
                                     uint64_t               timeout, 
                                     const pok_port_size_t  maxlen, 
                                     void*                  data, 
                                     pok_port_size_t*       len)
{

   pok_port_size_t   clen;
   pok_port_size_t   rlen;
   pok_port_size_t   toclean;
   pok_ret_t ret;

   pok_lockobj_lockattr_t lockattr;
   (void) lockattr;

   *len = 0;
   if (maxlen <= 0)
   {
      return POK_ERRNO_EINVAL;
   }

   if (id > POK_CONFIG_NB_PORTS)
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

   if (pok_ports[id].kind != POK_PORT_KIND_QUEUEING)
   {
      return POK_ERRNO_KIND;
   }

   if (pok_ports[id].direction != POK_PORT_DIRECTION_IN)
   {
      return POK_ERRNO_DIRECTION;
   }

   toclean = maxlen;
   if (toclean > pok_ports[id].size)
   {
      toclean =  pok_ports[id].size;
   }

   /* Make sure we clean the data buffer before */
   memset (data, '\0', toclean);

   /*
    * TODO : check that the partition has the right
    * to receive anything
    */

   if (timeout > 0)
   {
      timeout = timeout + POK_GETTICK();
   }

   ret = pok_lockobj_lock (&pok_ports[id].lock, NULL);

   if (ret != POK_ERRNO_OK)
   {
      return ret;
   }
   while (pok_ports[id].empty == TRUE)
   {
      if (timeout == 0)
      {
         pok_lockobj_unlock (&pok_ports[id].lock, NULL);
         return POK_ERRNO_EMPTY;
      }
      else
      {
         ret = pok_lockobj_eventwait (&pok_ports[id].lock, timeout);
         if (ret != POK_ERRNO_OK)
         {
            pok_lockobj_unlock (&pok_ports[id].lock, NULL);
            return ret;
         }
      }
   }

   clen = pok_port_consumed_size (id);

   if (maxlen > clen )
   {
      rlen = clen;
   }
   else
   {
      rlen = maxlen;
   }


   if (pok_port_get (id, data, rlen) == POK_ERRNO_OK)
   {
      *len =  rlen;
   }
   else
   {
      *len = 0;
   }

   pok_lockobj_unlock (&pok_ports[id].lock, NULL);

   return POK_ERRNO_OK;
}

#endif
