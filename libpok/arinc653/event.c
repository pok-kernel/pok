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


#ifdef POK_NEEDS_ARINC653_EVENT
#include <arinc653/types.h>
#include <arinc653/event.h>

#include <core/event.h>       /* For core function */

#include <libc/string.h>      /* For strcmp */

#include <errno.h>            /* For POK_ERRNO_... maccros */

#define CHECK_EVENTS_INIT if (pok_arinc653_events_initialized == 0) \
                       { \
                          uint16_t bla; \
                          for (bla = 0 ; bla < POK_CONFIG_ARINC653_NB_EVENTS ; bla++) \
                          {\
                             pok_arinc653_events_layers[bla].ready = 0;\
                          }\
                       }\
                       pok_arinc653_events_initialized = 1;




bool_t pok_arinc653_events_initialized = 0;

typedef struct
{
   pok_event_id_t    core_id;
   pok_bool_t        ready;
}pok_arinc653_event_layer_t;

extern char*                  pok_arinc653_events_names[POK_CONFIG_ARINC653_NB_EVENTS];
pok_arinc653_event_layer_t    pok_arinc653_events_layers[POK_CONFIG_NB_EVENTS];

void CREATE_EVENT (EVENT_NAME_TYPE EVENT_NAME,
                   EVENT_ID_TYPE *EVENT_ID,
                   RETURN_CODE_TYPE *RETURN_CODE)
{
   RETURN_CODE_TYPE  return_code_name;
   pok_event_id_t    core_id;
   pok_ret_t         core_ret;

   *RETURN_CODE = INVALID_CONFIG;

   CHECK_EVENTS_INIT

   GET_EVENT_ID (EVENT_NAME, EVENT_ID, &return_code_name);

   if (return_code_name == INVALID_CONFIG)
   {
      *RETURN_CODE = INVALID_CONFIG;
      return;
   }

   if (*EVENT_ID >= POK_CONFIG_ARINC653_NB_EVENTS)
   {
      *RETURN_CODE = INVALID_CONFIG;
      return;
   }

   if (pok_arinc653_events_layers[*EVENT_ID].ready)
   {
      *RETURN_CODE = NO_ACTION;
      return;
   }

   core_ret = pok_event_create (&core_id);

   /* DEBUG INFO
   printf("ID=%d\n", core_id);
   */

   if (core_ret != POK_ERRNO_OK)
   {
      *RETURN_CODE = INVALID_PARAM;
      return;
   }

   pok_arinc653_events_layers[*EVENT_ID].ready    = 1;
   pok_arinc653_events_layers[*EVENT_ID].core_id  = core_id;
   *RETURN_CODE = NO_ERROR;
}

void SET_EVENT (EVENT_ID_TYPE EVENT_ID,
                RETURN_CODE_TYPE *RETURN_CODE)
{
   pok_ret_t core_ret;

   *RETURN_CODE = INVALID_PARAM;

   CHECK_EVENTS_INIT


   if (EVENT_ID >= POK_CONFIG_ARINC653_NB_EVENTS)
   {
      *RETURN_CODE = INVALID_CONFIG;
      return;
   }

   if (pok_arinc653_events_layers[EVENT_ID].ready == 0)
   {
      *RETURN_CODE = NOT_AVAILABLE;
      return;
   }

   core_ret = pok_event_signal (pok_arinc653_events_layers[EVENT_ID].core_id);
   if (core_ret == POK_ERRNO_OK)
   {
      *RETURN_CODE = NO_ERROR;
      return;
   }
   else
   {
      *RETURN_CODE = INVALID_PARAM;
      return;
   }
}

void RESET_EVENT (EVENT_ID_TYPE EVENT_ID,
                  RETURN_CODE_TYPE *RETURN_CODE)
{
   (void) EVENT_ID;
   (void) RETURN_CODE;

   /* Not yet implemented */
}

void WAIT_EVENT (EVENT_ID_TYPE EVENT_ID,
                 SYSTEM_TIME_TYPE TIME_OUT,
                 RETURN_CODE_TYPE *RETURN_CODE)
{
   pok_ret_t core_ret;

   *RETURN_CODE = INVALID_PARAM;

   CHECK_EVENTS_INIT

   if (EVENT_ID >= POK_CONFIG_ARINC653_NB_EVENTS)
   {
      *RETURN_CODE = INVALID_CONFIG;
      return;
   }

   if (pok_arinc653_events_layers[EVENT_ID].ready == 0)
   {
      *RETURN_CODE = NOT_AVAILABLE;
      return;
   }

   core_ret = pok_event_wait (pok_arinc653_events_layers[EVENT_ID].core_id, TIME_OUT);

   switch (core_ret)
   {
      case POK_ERRNO_OK:
         *RETURN_CODE = NO_ERROR;
         break;

      case POK_ERRNO_TIMEOUT:
         *RETURN_CODE = TIMED_OUT;
         break;

      default:
         *RETURN_CODE = INVALID_PARAM;
         break;
   }
}

void GET_EVENT_ID (EVENT_NAME_TYPE EVENT_NAME,
                   EVENT_ID_TYPE *EVENT_ID,
                   RETURN_CODE_TYPE *RETURN_CODE)
{
   uint16_t i;
   uint16_t len;

   *RETURN_CODE = INVALID_CONFIG;

   len = strlen (EVENT_NAME);

   CHECK_EVENTS_INIT

   for (i = 0 ; i < POK_CONFIG_ARINC653_NB_EVENTS ; i++)
   {
      if (strncmp (EVENT_NAME, pok_arinc653_events_names[i], len) == 0)
      {
         *EVENT_ID = i;
         *RETURN_CODE = NO_ERROR;
         return;
      }
   }
}

void GET_EVENT_STATUS (EVENT_ID_TYPE EVENT_ID,
                       EVENT_STATUS_TYPE *EVENT_STATUS,
                       RETURN_CODE_TYPE *RETURN_CODE)
{
   (void) EVENT_ID;
   (void) EVENT_STATUS;
   (void) RETURN_CODE;
}

#endif
