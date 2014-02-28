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
 * Created by julien on Thu Jan 29 15:10:33 2009 
 */

#ifndef __POK_ASSERT_H__
#define __POK_ASSERT_H__

#include <errno.h>
#include <libc/stdio.h>

#define ASSERT_RET(ret) if (ret != POK_ERRNO_OK) { printf ("ASSERTION FAILED, ret=%d, file=%s, line=%d\n", ret, __FILE__, __LINE__);}
#define ASSERT_RET_WITH_EXCEPTION(ret,but) if ((ret != POK_ERRNO_OK)&&(ret != but)) { printf ("ASSERTION FAILED, ret=%d, file=%s, line=%d\n", ret, __FILE__, __LINE__);}

#endif

