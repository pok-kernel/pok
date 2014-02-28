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


#ifndef  __POK_USER_BLACKBOARD_H__
#define __POK_USER_BLACKBOARD_H__

#ifdef POK_NEEDS_MIDDLEWARE
#ifdef POK_NEEDS_BLACKBOARDS

#include <types.h>
#include <errno.h>

typedef struct
{
   pok_size_t          size;
   pok_bool_t          empty;
   pok_range_t         waiting_processes;
   pok_size_t          index;
   pok_bool_t          ready;
   pok_event_id_t      lock;
}pok_blackboard_t;

typedef struct
{
   pok_port_size_t      msg_size;
   pok_bool_t           empty;
   pok_range_t          waiting_processes;
}pok_blackboard_status_t;


pok_ret_t pok_blackboard_create (char*                   name,
                                 const pok_size_t        msg_size,
                                 pok_blackboard_id_t*    id);

pok_ret_t pok_blackboard_read (const pok_blackboard_id_t      id,
                               const uint64_t                 timeout,
                               void*                          data,
                               pok_port_size_t*               len);

pok_ret_t pok_blackboard_display (const pok_blackboard_id_t       id,
                                  const void*                     message,
                                  const pok_port_size_t           len);

pok_ret_t pok_blackboard_clear (const pok_blackboard_id_t               id);

pok_ret_t pok_blackboard_id     (char*                            name,
                                 pok_blackboard_id_t*             id);

pok_ret_t pok_blackboard_status (const pok_blackboard_id_t        id,
                                 pok_blackboard_status_t*         status);
#endif
#endif
#endif

