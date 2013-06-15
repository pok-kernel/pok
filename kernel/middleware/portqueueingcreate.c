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

#include <errno.h>
#include <types.h>
#include <core/lockobj.h>
#include <middleware/port.h>
#include <middleware/queue.h>

extern pok_port_t    pok_ports[POK_CONFIG_NB_PORTS];

pok_ret_t pok_port_queueing_create (char*                                  name, 
                                    const pok_port_size_t                  size, 
                                    const pok_port_direction_t             direction,
                                    const pok_port_queueing_discipline_t   discipline,
                                    pok_port_id_t*                         id)
{
   pok_ret_t            ret;
   pok_lockobj_attr_t   lockattr;
   if (discipline != POK_PORT_QUEUEING_DISCIPLINE_FIFO)
   {
      return POK_ERRNO_DISCIPLINE;
   }

   ret = pok_port_create (name, size, direction, POK_PORT_KIND_QUEUEING, id);

   /*
    * FIXME: should be done using another way by reinitializing
    * ports when a partition is restarted. At this time, when
    * a partition is restarted, the ports are not reinitialized
    * so that when the partition restart, it tries to reopen
    * ports already created. We pass over actually and keep
    * the port in the previous step, but we should reinitialize
    * the port while reinitializing the partition AND also
    * does not report an OK return code (see below) when trying to re
    * create a port that was already created.
    */
   if (ret == POK_ERRNO_EXISTS)
   {
      return POK_ERRNO_OK;
   }

   if (ret != POK_ERRNO_OK)
   {
      return ret;
   }

   pok_ports[*id].discipline = discipline;
   
   lockattr.kind              = POK_LOCKOBJ_KIND_EVENT;
   lockattr.locking_policy    = POK_LOCKOBJ_POLICY_STANDARD;
   ret = pok_lockobj_create (&pok_ports[*id].lock, &lockattr);

   return (ret);
}

#endif
