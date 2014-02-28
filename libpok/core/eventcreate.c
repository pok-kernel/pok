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

#if defined (POK_NEEDS_EVENTS) || defined (POK_NEEDS_BUFFERS) || defined (POK_NEEDS_BLACKBOARDS)

#include <types.h>
#include <errno.h>
#include <core/event.h>
#include <core/lockobj.h>
#include <core/syscall.h>

pok_ret_t pok_event_create (pok_event_id_t* id)
{
   pok_lockobj_attr_t lockattr;
   
   lockattr.kind = POK_LOCKOBJ_KIND_EVENT;

   lockattr.locking_policy = POK_LOCKOBJ_POLICY_STANDARD;

   return pok_syscall2 (POK_SYSCALL_LOCKOBJ_CREATE, (uint32_t)id, (uint32_t)&lockattr);
}

#endif

