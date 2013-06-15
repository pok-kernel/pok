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


#ifdef POK_NEEDS_MUTEXES
#include <types.h>
#include <errno.h>
#include <core/mutex.h>
#include <core/lockobj.h>
#include <core/syscall.h>

pok_ret_t pok_mutex_create (pok_mutex_id_t* id, pok_mutex_attr_t* attr)
{
   pok_lockobj_attr_t lockattr;
   
   lockattr.kind = POK_LOCKOBJ_KIND_MUTEX;
   if (attr == NULL)
   {
      lockattr.locking_policy = POK_MUTEX_POLICY_STANDARD;
   }
   else
   {
      switch (attr->policy)
      {
         case POK_MUTEX_POLICY_STANDARD:
            lockattr.locking_policy = POK_MUTEX_POLICY_STANDARD;
            break;

         case POK_MUTEX_POLICY_PIP:
            lockattr.locking_policy = POK_MUTEX_POLICY_PIP;
            break;

         case POK_MUTEX_POLICY_PCP:
            lockattr.locking_policy = POK_MUTEX_POLICY_PCP;
            break;
         
         default:
            lockattr.locking_policy = POK_MUTEX_POLICY_STANDARD;
      }
   }
   return pok_syscall2 (POK_SYSCALL_LOCKOBJ_CREATE, (uint32_t)id, (uint32_t)&lockattr);
}

#endif
