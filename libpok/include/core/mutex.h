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


#ifndef __POK_KERNEL_MUTEX_H__
#define __POK_KERNEL_MUTEX_H__

#include <types.h>
#include <errno.h>

typedef enum
{
   POK_MUTEX_POLICY_STANDARD = 0,
   POK_MUTEX_POLICY_PIP      = 1,
   POK_MUTEX_POLICY_PCP      = 2
}pok_mutex_policy_t;

typedef struct
{
   pok_mutex_policy_t policy; 
}pok_mutex_attr_t;

pok_ret_t pok_mutex_create (pok_mutex_id_t* id, pok_mutex_attr_t* attr);
pok_ret_t pok_mutex_lock   (pok_mutex_id_t id);
pok_ret_t pok_mutex_trylock (pok_mutex_id_t id, const uint64_t time);
pok_ret_t pok_mutex_unlock (pok_mutex_id_t id);

#endif
