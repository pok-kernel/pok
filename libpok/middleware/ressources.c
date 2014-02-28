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
#include <middleware/blackboard.h>
#include <middleware/buffer.h>

#ifdef POK_NEEDS_BUFFERS
pok_buffer_t pok_buffers[POK_CONFIG_NB_BUFFERS];
const char pok_buffers_data[1024]; /* FIXME : correct size of buffers ! */
#endif

#ifdef POK_NEEDS_BLACKBOARDS
pok_blackboard_t pok_blackboards[POK_CONFIG_NB_BLACKBOARDS];
const char pok_blackboards_data[1024]; /* FIXME : correct size of blackboard buffers ! */
#endif

#endif
