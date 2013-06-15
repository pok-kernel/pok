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

#include <errno.h>

#ifdef POK_NEEDS_BLACKBOARDS
   #include <middleware/blackboard.h>
   pok_ret_t pok_blackboard_init (void);
#endif

#ifdef POK_NEEDS_BUFFERS
   #include <middleware/buffer.h>
   pok_ret_t pok_buffer_init (void);
#endif

int main();

int __pok_partition_start ()
{
#ifdef POK_NEEDS_MIDDLEWARE

#ifdef POK_NEEDS_BLACKBOARDS
   pok_blackboard_init ();
#endif

#ifdef POK_NEEDS_BUFFERS
   pok_buffer_init ();
#endif

#endif /* POK_NEEDS_MIDDLEWARE */
   main(); /* main loop from user */
   return (0);
}
