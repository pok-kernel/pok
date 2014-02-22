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

#if defined (POK_NEEDS_PORTS_SAMPLING) || defined (POK_NEEDS_PORTS_QUEUEING)
#include <types.h>
#include <core/debug.h>
#include <core/error.h>
#include <middleware/port.h>

extern pok_port_t pok_ports[POK_CONFIG_NB_PORTS];

void pok_port_init (void)
{
   uint8_t i;

#ifdef POK_NEEDS_PARTITIONS
#if defined (POK_NEEDS_DEBUG) || defined (POK_NEEDS_ERROR_HANDLING)
   for (i = 0 ; i < POK_CONFIG_NB_PORTS ; i++)
   {
      if ((((uint8_t[]) POK_CONFIG_PARTITIONS_PORTS)[i]) >= POK_CONFIG_NB_PARTITIONS)
      {
#ifdef POK_NEEDS_DEBUG
         printf ("Invalid configuration, port %d owned by an unknown partition\n", i);
#endif
#ifdef POK_NEEDS_ERROR_HANDLING
         pok_kernel_error (POK_ERROR_KIND_KERNEL_CONFIG);
#endif
      }
   }
#endif
#endif

   for (i = 0 ; i < POK_CONFIG_NB_PORTS ; i++)
   {
      pok_ports[i].size    = 0;
      pok_ports[i].off_b   = 0;
      pok_ports[i].off_e   = 0;
      pok_ports[i].empty   = TRUE;
      pok_ports[i].full    = FALSE;
      pok_ports[i].index   = 0;
      pok_ports[i].ready   = FALSE;
      pok_ports[i].kind    = POK_PORT_KIND_INVALID;
      pok_ports[i].last_receive = 0;
      pok_ports[i].must_be_flushed = FALSE;
   }
}

#endif
