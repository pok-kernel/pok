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


#ifdef POK_NEEDS_ARINC653_PARTITION

#include <arinc653/types.h>
#include <arinc653/partition.h>

#include <core/partition.h>

#include <errno.h>

void GET_PARTITION_STATUS (PARTITION_STATUS_TYPE *partition_status,
                           RETURN_CODE_TYPE      *return_code)
{
  pok_current_partition_get_id(&partition_status->IDENTIFIER);
  pok_current_partition_get_period(&partition_status->PERIOD);
  pok_current_partition_get_duration(&partition_status->DURATION);
  pok_current_partition_get_lock_level(&partition_status->LOCK_LEVEL);
  pok_current_partition_get_operating_mode(&partition_status->OPERATING_MODE);
  pok_current_partition_get_start_condition(&partition_status->START_CONDITION);
  *return_code = NO_ERROR;
}

void SET_PARTITION_MODE (OPERATING_MODE_TYPE operating_mode,
                         RETURN_CODE_TYPE *return_code)
{
  pok_partition_mode_t core_mode;
  pok_ret_t            core_ret;
  pok_partition_mode_t current_mode;

  pok_current_partition_get_operating_mode(&current_mode);
   switch (operating_mode)
   {
      case IDLE:
         core_mode = POK_PARTITION_MODE_IDLE;
         break;

      case NORMAL:
         core_mode = POK_PARTITION_MODE_NORMAL;
         break;

      case COLD_START:
         core_mode = POK_PARTITION_MODE_INIT_COLD;
         *return_code = NOT_AVAILABLE;
	 return;

      case WARM_START:
         core_mode = POK_PARTITION_MODE_INIT_WARM;
         break;

      default:
         *return_code = INVALID_PARAM;
         return;
   }

   if (current_mode == core_mode)
     {
       *return_code = NO_ACTION;
       return ;
     }
   if (current_mode == POK_PARTITION_MODE_INIT_COLD &&
       core_mode == POK_PARTITION_MODE_INIT_WARM)
     {
       *return_code = INVALID_MODE;
       return ;
     }
   core_ret = pok_partition_set_mode (core_mode);
   *return_code = core_ret;
}

#endif
