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
#ifdef POK_NEEDS_BLACKBOARDS

#include <errno.h>
#include <types.h>
#include <libc/string.h>
#include <core/event.h>
#include <middleware/blackboard.h>

extern pok_blackboard_t    pok_blackboards[POK_CONFIG_NB_BLACKBOARDS];
extern char*               pok_blackboards_names[POK_CONFIG_NB_BLACKBOARDS];
pok_size_t                 pok_blackboards_data_index = 0;

pok_ret_t pok_blackboard_create (char*                             name, 
                                 const pok_port_size_t             msg_size, 
                                 pok_blackboard_id_t*              id)
{
   pok_ret_t   ret;
   uint8_t     n;

   for (n=0 ; n < POK_CONFIG_NB_BLACKBOARDS ; n++)
   {
      if (streq (name, pok_blackboards_names[n]))
      {
         if (pok_blackboards[n].ready == TRUE)
         {
            return POK_ERRNO_READY;
         }

         ret = pok_event_create (&pok_blackboards[n].lock);

         if (ret != POK_ERRNO_OK)
         {
            return ret;
         }

         pok_blackboards[n].ready               = TRUE;
         pok_blackboards[n].empty               = TRUE;
         pok_blackboards[n].index               = pok_blackboards_data_index;
         pok_blackboards[n].waiting_processes   = 0;
         pok_blackboards[n].size                = msg_size;
         *id                                    = n;
         pok_blackboards_data_index             = pok_blackboards_data_index + msg_size;
         return POK_ERRNO_OK;
      }
   }

   return POK_ERRNO_EINVAL;
}

#endif /* POK_NEEDS_BLACKBOARDS */
#endif /* POK_NEEDS_MIDDLEWARE */
