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


#ifdef POK_NEEDS_MIDDLEWARE
#ifdef POK_NEEDS_BUFFERS

#include <middleware/buffer.h>
#include <errno.h>
#include <types.h>

extern pok_buffer_t    pok_buffers[POK_CONFIG_NB_BUFFERS];

pok_ret_t pok_buffer_init (void)
{
   uint8_t n;

   for (n=0 ; n < POK_CONFIG_NB_BUFFERS ; n++)
   {
      pok_buffers[n].ready                = FALSE;
      pok_buffers[n].full                 = FALSE;
      pok_buffers[n].empty                = TRUE;
   }

   return POK_ERRNO_OK;
}

#endif /* POK_NEEDS_BLACKBOARDS */
#endif
