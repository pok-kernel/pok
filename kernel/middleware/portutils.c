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

/**
 * \file    middleware/portutils.c
 * \date    2008-2009
 * \brief   Various functions for ports management.
 * \author  Julien Delange
 */

#if defined (POK_NEEDS_PORTS_SAMPLING) || defined (POK_NEEDS_PORTS_QUEUEING)
#include <types.h>
#include <libc.h>
#include <core/time.h>
#include <middleware/port.h>
#include <middleware/queue.h>

extern pok_port_t    pok_ports[POK_CONFIG_NB_PORTS];
extern pok_queue_t   pok_queue;
extern uint8_t       pok_current_partition;

pok_port_size_t pok_port_available_size (uint8_t pid)
{
   if (pok_ports[pid].full == TRUE)
   {
      return 0;
   }

   if (pok_ports[pid].off_b < pok_ports[pid].off_e)
   {
      return (pok_ports[pid].off_b - pok_ports[pid].off_e);
   }
   else
   {
      return (pok_ports[pid].size - pok_ports[pid].off_e + pok_ports[pid].off_b);
   }
}

pok_port_size_t pok_port_consumed_size (uint8_t pid)
{
   if (pok_ports[pid].empty == TRUE)
   {
      return 0;
   }

   if (pok_ports[pid].off_b < pok_ports[pid].off_e )
   {
      return (pok_ports[pid].off_e - pok_ports[pid].off_b);
   }
   else
   {
      return (pok_ports[pid].size - pok_ports[pid].off_b + pok_ports[pid].off_e);
   }
}


pok_ret_t pok_port_get (const uint32_t pid, void *data, const pok_port_size_t size)
{

#ifdef POK_NEEDS_PORTS_QUEUEING
   pok_port_size_t tmp_size;
   pok_port_size_t tmp_size2;
#endif

   switch (pok_ports[pid].kind)
   {

#ifdef POK_NEEDS_PORTS_QUEUEING
      case POK_PORT_KIND_QUEUEING:
         if (pok_ports[pid].empty == TRUE)
         {
            return POK_ERRNO_EINVAL;
         }

         if (pok_ports[pid].size < size)
         {
            return POK_ERRNO_SIZE;
         }

         if ((pok_ports[pid].off_b + size) > pok_ports[pid].size)
         {
            tmp_size = pok_ports[pid].size - pok_ports[pid].off_b;
            memcpy (data, &pok_queue.data[pok_ports[pid].index + pok_ports[pid].off_b], tmp_size);
            tmp_size2 = size - tmp_size;
            memcpy (data + tmp_size, &pok_queue.data[pok_ports[pid].index], tmp_size2);
         }
         else
         {
            memcpy (data, &pok_queue.data[pok_ports[pid].index + pok_ports[pid].off_b], size);
         }

         pok_ports[pid].off_b = (pok_ports[pid].off_b + size) % pok_ports[pid].size;

         if (pok_ports[pid].off_b == pok_ports[pid].off_e)
         {
            pok_ports[pid].empty = TRUE;
            pok_ports[pid].full  = FALSE;
         }

         return POK_ERRNO_OK;
         break;
#endif

#ifdef POK_NEEDS_PORTS_SAMPLING
      case POK_PORT_KIND_SAMPLING:
         if (pok_ports[pid].empty == TRUE)
         {
            return POK_ERRNO_EMPTY;
         }

         if (size > pok_ports[pid].size)
         {
            return POK_ERRNO_SIZE;
         }

         memcpy (data, &pok_queue.data[pok_ports[pid].index + pok_ports[pid].off_b], size);

         return POK_ERRNO_OK;
         break;
#endif

      default:
         return POK_ERRNO_EINVAL;
   }
}

pok_ret_t  pok_port_write (const uint8_t pid, const void *data, const pok_port_size_t size)
{
#ifdef POK_NEEDS_PORTS_QUEUEING
   pok_port_size_t tmp_size;
   pok_port_size_t tmp_size2;
#endif

   switch (pok_ports[pid].kind)
   {
#ifdef POK_NEEDS_PORTS_QUEUEING
      case POK_PORT_KIND_QUEUEING:
         if (pok_ports[pid].full == TRUE)
         {
            return POK_ERRNO_SIZE;
         }

         if (size > pok_ports[pid].size)
         {
            return POK_ERRNO_SIZE;
         }

         if ((pok_ports[pid].off_e + size) > pok_ports[pid].size)
         {
            tmp_size = pok_ports[pid].size - pok_ports[pid].off_e;
            memcpy (&pok_queue.data[pok_ports[pid].index + pok_ports[pid].off_e], data, tmp_size);

            tmp_size2 = size - tmp_size;
            memcpy (&pok_queue.data[pok_ports[pid].index], data + tmp_size, tmp_size2);
         }
         else
         {
            memcpy (&pok_queue.data[pok_ports[pid].index + pok_ports[pid].off_e], data, size);
         }

         pok_ports[pid].off_e = (pok_ports[pid].off_e + size) % pok_ports[pid].size;

         if (pok_ports[pid].off_e == pok_ports[pid].off_b)
         {
            pok_ports[pid].full = TRUE;
         }

         pok_ports[pid].empty = FALSE;

         return POK_ERRNO_OK;

         break;
#endif

#ifdef POK_NEEDS_PORTS_SAMPLING
      case POK_PORT_KIND_SAMPLING:

         if (size > pok_ports[pid].size)
         {
            return POK_ERRNO_SIZE;
         }

         memcpy (&pok_queue.data[pok_ports[pid].index + pok_ports[pid].off_e], data, size);

         pok_ports[pid].empty = FALSE;
         pok_ports[pid].last_receive = POK_GETTICK ();

         return POK_ERRNO_OK;

         break;
#endif

      default:
         return POK_ERRNO_EINVAL;
   }
}


/*
 * This function is designed to transfer data from one port to another
 * It is called when we transfer all data from one partition to the
 * others.
 */
pok_ret_t pok_port_transfer (const uint8_t pid_dst, const uint8_t pid_src)
{
   pok_port_size_t len = 0;
   pok_port_size_t src_len_consumed = 0;

   if (pok_ports[pid_src].empty == TRUE)
   {
      return POK_ERRNO_EMPTY;
   }

   if (pok_ports[pid_src].kind == POK_PORT_KIND_QUEUEING)
   {
      len = pok_port_available_size (pid_dst);
   }
   else
   {
      if (pok_ports[pid_src].size != pok_ports[pid_dst].size)
      {
         return POK_ERRNO_SIZE;
      }

      len = pok_ports[pid_src].size;
   }

   if (pok_ports[pid_src].kind == POK_PORT_KIND_QUEUEING)
   {
      src_len_consumed = pok_port_consumed_size (pid_src);

      if (src_len_consumed == 0)
      {
         return POK_ERRNO_SIZE;
      }

      if (len > src_len_consumed)
      {
         len = src_len_consumed;
      }
      /*
       * Here, we check the size of data produced in the source port.
       * If there is more free space in the destination port, the size
       * of copied data will be the occupied size in the source port.
       */
   }

   if (len == 0)
   {
      return POK_ERRNO_SIZE;
   }
   /*
    * Len is the size to copy. If size is null, it's better to return
    * directly
    */

   memcpy (&pok_queue.data[pok_ports[pid_dst].index + pok_ports[pid_dst].off_e], &pok_queue.data[pok_ports[pid_src].index + pok_ports[pid_src].off_b], len);

   if (pok_ports[pid_src].kind == POK_PORT_KIND_QUEUEING)
   {
      pok_ports[pid_dst].off_e =  (pok_ports[pid_dst].off_e + len) % pok_ports[pid_dst].size;
      pok_ports[pid_src].off_b =  (pok_ports[pid_src].off_b + len) % pok_ports[pid_src].size;

      if (pok_ports[pid_src].off_b == pok_ports[pid_src].off_e)
      {
         pok_ports[pid_src].empty = TRUE;
         pok_ports[pid_src].full  = FALSE;
      }
   }
   else
   {
      pok_ports[pid_src].empty = TRUE;
   }

   pok_ports[pid_src].full  = FALSE;
   pok_ports[pid_dst].empty = FALSE;

   return POK_ERRNO_OK;
}

bool_t pok_own_port (const uint8_t partition, const uint8_t port)
{
   if (port > POK_CONFIG_NB_PORTS)
   {
      return FALSE;
   }

#ifdef POK_CONFIG_PARTITIONS_PORTS
   if ((((uint8_t[]) POK_CONFIG_PARTITIONS_PORTS)[port]) == partition)
   {
      return TRUE;
   }
#endif

   return FALSE;
}

#endif
