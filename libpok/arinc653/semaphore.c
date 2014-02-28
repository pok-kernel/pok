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

/**
 * \file semaphore.c
 * \brief Provides ARINC653 API functionnalities for semaphore management.
 */

#ifdef POK_NEEDS_ARINC653_SEMAPHORE

#include <types.h>
#include <libc/string.h>
#include <arinc653/types.h>
#include <arinc653/semaphore.h>
#include <core/semaphore.h>

#define CHECK_SEM_INIT if (pok_arinc653_semaphores_initialized == 0) \
                       { \
                          uint16_t bla; \
                          for (bla = 0 ; bla < POK_CONFIG_ARINC653_NB_SEMAPHORES ; bla++) \
                          {\
                             pok_arinc653_semaphores_layers[bla].ready = 0;\
                          }\
                       }\
                       pok_arinc653_semaphores_initialized = 1;

pok_bool_t           pok_arinc653_semaphores_initialized = 0;

typedef struct
{
   pok_bool_t        ready;
   pok_sem_id_t      core_id;
}pok_arinc653_semaphore_layer_t;

extern char*                           pok_arinc653_semaphores_names[POK_CONFIG_ARINC653_NB_SEMAPHORES];
pok_arinc653_semaphore_layer_t         pok_arinc653_semaphores_layers[POK_CONFIG_ARINC653_NB_SEMAPHORES];

void CREATE_SEMAPHORE (SEMAPHORE_NAME_TYPE SEMAPHORE_NAME,
                       SEMAPHORE_VALUE_TYPE CURRENT_VALUE,
                       SEMAPHORE_VALUE_TYPE MAXIMUM_VALUE,
                       QUEUING_DISCIPLINE_TYPE QUEUING_DISCIPLINE,
                       SEMAPHORE_ID_TYPE *SEMAPHORE_ID,
                       RETURN_CODE_TYPE *RETURN_CODE )
{

   RETURN_CODE_TYPE  return_code_name;
   pok_sem_id_t      sem_id;
   pok_ret_t         core_ret;

   CHECK_SEM_INIT;

   *RETURN_CODE = INVALID_CONFIG;

   GET_SEMAPHORE_ID (SEMAPHORE_NAME, SEMAPHORE_ID, &return_code_name);

   if (return_code_name == INVALID_CONFIG)
   {
      *RETURN_CODE = INVALID_CONFIG;
      return;
   }


   if (*SEMAPHORE_ID > POK_CONFIG_ARINC653_NB_SEMAPHORES)
   {
      *RETURN_CODE = INVALID_CONFIG;
      return;
   }

   if (pok_arinc653_semaphores_layers[*SEMAPHORE_ID].ready)
   {
      *RETURN_CODE = NO_ACTION;
      return;
   }

   core_ret = pok_sem_create (&sem_id, CURRENT_VALUE, MAXIMUM_VALUE, QUEUING_DISCIPLINE);

   if (core_ret != POK_ERRNO_OK)
   {
      *RETURN_CODE = INVALID_PARAM;
      return;
   }

   pok_arinc653_semaphores_layers[*SEMAPHORE_ID].ready     = 1;
   pok_arinc653_semaphores_layers[*SEMAPHORE_ID].core_id   = sem_id;

   *RETURN_CODE = NO_ERROR;
}

void WAIT_SEMAPHORE (SEMAPHORE_ID_TYPE SEMAPHORE_ID,
                     SYSTEM_TIME_TYPE TIME_OUT,
                     RETURN_CODE_TYPE *RETURN_CODE )
{
   pok_ret_t core_ret;

   CHECK_SEM_INIT;

   if (SEMAPHORE_ID > POK_CONFIG_ARINC653_NB_SEMAPHORES)
   {
      *RETURN_CODE = INVALID_PARAM;
      return;
   }

   core_ret = pok_sem_wait (pok_arinc653_semaphores_layers[SEMAPHORE_ID].core_id, TIME_OUT);

   *RETURN_CODE = INVALID_PARAM;

   if (core_ret == POK_ERRNO_OK)
   {
      *RETURN_CODE = NO_ERROR;
   }

   if (core_ret == POK_ERRNO_TIMEOUT)
   {
      *RETURN_CODE = TIMED_OUT;
   }
}

void SIGNAL_SEMAPHORE (SEMAPHORE_ID_TYPE SEMAPHORE_ID,
                       RETURN_CODE_TYPE *RETURN_CODE )
{
   pok_ret_t core_ret;

   CHECK_SEM_INIT;

   if (SEMAPHORE_ID > POK_CONFIG_ARINC653_NB_SEMAPHORES)
   {
      *RETURN_CODE = INVALID_PARAM;
      return;
   }

   core_ret = pok_sem_signal (pok_arinc653_semaphores_layers[SEMAPHORE_ID].core_id);

   if (core_ret == POK_ERRNO_OK)
   {
      *RETURN_CODE = NO_ERROR;
   }
   else
   {
      *RETURN_CODE = INVALID_PARAM;
   }
}

void GET_SEMAPHORE_ID (SEMAPHORE_NAME_TYPE SEMAPHORE_NAME,
                       SEMAPHORE_ID_TYPE *SEMAPHORE_ID,
                       RETURN_CODE_TYPE *RETURN_CODE )
{
   uint16_t i;
   uint16_t len;

   CHECK_SEM_INIT;

   len = strlen (SEMAPHORE_NAME);

   *RETURN_CODE = INVALID_CONFIG;

   for (i = 0 ; i < POK_CONFIG_ARINC653_NB_SEMAPHORES ; i++)
   {
      if (strncmp (pok_arinc653_semaphores_names[i], SEMAPHORE_NAME, len) == 0)
      {
         *SEMAPHORE_ID = i;
         *RETURN_CODE = NO_ERROR;
         break;
      }
   }
}

void GET_SEMAPHORE_STATUS (SEMAPHORE_ID_TYPE SEMAPHORE_ID,
                           SEMAPHORE_STATUS_TYPE *SEMAPHORE_STATUS,
                           RETURN_CODE_TYPE *RETURN_CODE )
{
   (void) SEMAPHORE_ID;
   (void) SEMAPHORE_STATUS;

   CHECK_SEM_INIT;

   *RETURN_CODE = NO_ERROR;
}

#endif
