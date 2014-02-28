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
 * Created by julien on Fri Aug 21 16:14:45 2009 
 */

#include <core/dependencies.h>

#ifdef POK_NEEDS_ERROR_HANDLING
#include <types.h>
#include <core/time.h>
#include <core/error.h>

#ifdef POK_NEEDS_DEBUG
#include <libc/stdio.h>
#endif

pok_error_report_t pok_error_reported[POK_ERROR_MAX_LOGGED];
uint16_t           pok_error_nb_reported = 0;

void pok_error_log (const uint32_t error_id, const uint32_t thread_id)
{

#ifdef POK_NEEDS_DEBUG
   printf ("Log error %d for thread %d\n", error_id, thread_id);
#endif

   /**
    * log the error
    */
   pok_error_reported[pok_error_nb_reported].thread = thread_id;
   pok_error_reported[pok_error_nb_reported].error  = error_id;
   pok_time_gettick (&pok_error_reported[pok_error_nb_reported].when);

   pok_error_nb_reported++;
}
#endif
