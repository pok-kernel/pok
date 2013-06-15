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


#ifdef POK_NEEDS_ARINC653_BLACKBOARD

#include <arinc653/types.h>
#include <arinc653/blackboard.h>
#include <middleware/blackboard.h>

 
void CREATE_BLACKBOARD ( 
       /*in */ BLACKBOARD_NAME_TYPE     BLACKBOARD_NAME, 
       /*in */ MESSAGE_SIZE_TYPE        MAX_MESSAGE_SIZE, 
       /*out*/ BLACKBOARD_ID_TYPE       *BLACKBOARD_ID, 
       /*out*/ RETURN_CODE_TYPE         *RETURN_CODE )
{
   pok_blackboard_id_t  core_id;
   pok_ret_t            core_ret;

   core_ret = pok_blackboard_create (BLACKBOARD_NAME, MAX_MESSAGE_SIZE, &core_id);
   *RETURN_CODE = core_ret;
}
 
void DISPLAY_BLACKBOARD ( 
       /*in */ BLACKBOARD_ID_TYPE       BLACKBOARD_ID, 
       /*in */ MESSAGE_ADDR_TYPE        MESSAGE_ADDR,       /* by reference */ 
       /*in */ MESSAGE_SIZE_TYPE        LENGTH, 
       /*out*/ RETURN_CODE_TYPE         *RETURN_CODE )
{
   pok_ret_t core_ret;
   core_ret = pok_blackboard_display (BLACKBOARD_ID, MESSAGE_ADDR, LENGTH);
   *RETURN_CODE = core_ret;
}
 
void READ_BLACKBOARD ( 
       /*in */ BLACKBOARD_ID_TYPE       BLACKBOARD_ID, 
       /*in */ SYSTEM_TIME_TYPE         TIME_OUT, 
       /*out*/ MESSAGE_ADDR_TYPE        MESSAGE_ADDR, 
       /*out*/ MESSAGE_SIZE_TYPE        *LENGTH, 
       /*out*/ RETURN_CODE_TYPE         *RETURN_CODE )
{
   pok_ret_t core_ret;
   core_ret = pok_blackboard_read (BLACKBOARD_ID, TIME_OUT, MESSAGE_ADDR, LENGTH);
   *RETURN_CODE = core_ret;
}
 
void CLEAR_BLACKBOARD ( 
       /*in */ BLACKBOARD_ID_TYPE       BLACKBOARD_ID, 
       /*out*/ RETURN_CODE_TYPE         *RETURN_CODE )
{
   (void) BLACKBOARD_ID;
   *RETURN_CODE = NOT_AVAILABLE;
}
 
void GET_BLACKBOARD_ID ( 
       /*in */ BLACKBOARD_NAME_TYPE     BLACKBOARD_NAME, 
       /*out*/ BLACKBOARD_ID_TYPE       *BLACKBOARD_ID, 
       /*out*/ RETURN_CODE_TYPE         *RETURN_CODE )
{
   (void) BLACKBOARD_NAME;
   (void) BLACKBOARD_ID;
   *RETURN_CODE = NOT_AVAILABLE;
}
 
void GET_BLACKBOARD_STATUS ( 
       /*in */ BLACKBOARD_ID_TYPE       BLACKBOARD_ID, 
       /*out*/ BLACKBOARD_STATUS_TYPE   *BLACKBOARD_STATUS, 
       /*out*/ RETURN_CODE_TYPE         *RETURN_CODE )
{
   (void) BLACKBOARD_ID;
   (void) BLACKBOARD_STATUS;
   *RETURN_CODE = NOT_AVAILABLE;
}
#endif
