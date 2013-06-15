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
 * Created by julien on Mon Apr  6 21:33:22 2009 
 */

#ifndef __LIBPOK_DEPENDENCIES_H__
#define __LIBPOK_DEPENDENCIES_H__


#ifdef POK_CONFIG_OPTIMIZE_FOR_GENERATED_CODE

#ifdef POK_NEEDS_LIBC_STDIO
   #ifndef POK_CONFIG_NEEDS_FUNC_MEMCPY
   #define POK_CONFIG_NEEDS_FUNC_MEMCPY 1
   #endif

   #ifndef POK_CONFIG_NEEDS_FUNC_PRINTF
   #define POK_CONFIG_NEEDS_FUNC_PRINTF 1
   #endif

   #ifndef POK_CONFIG_NEEDS_FUNC_VPRINTF
   #define POK_CONFIG_NEEDS_FUNC_VPRINTF 1
   #endif
#endif

#ifdef POK_NEEDS_LIBC_STDLIB
   #ifndef POK_CONFIG_NEEDS_FUNC_MEMCPY
   #define POK_CONFIG_NEEDS_FUNC_MEMCPY 1
   #endif

   #ifndef POK_NEEDS_ALLOCATOR
   #define POK_NEEDS_ALLOCATOR                  1
   #endif
   
   #ifndef POK_CONFIG_NEEDS_FUNC_RAND
   #define POK_CONFIG_NEEDS_FUNC_RAND           1
   #endif

   #ifndef POK_CONFIG_NEEDS_FUNC_MALLOC
   #define POK_CONFIG_NEEDS_FUNC_MALLOC         1
   #endif

   #ifndef POK_CONFIG_NEEDS_FUNC_CALLOC
   #define POK_CONFIG_NEEDS_FUNC_CALLOC         1
   #endif

   #ifndef POK_CONFIG_NEEDS_FUNC_FREE
   #define POK_CONFIG_NEEDS_FUNC_FREE           1
   #endif
#endif /* POK_NEEDS_STDLIB */

#ifdef POK_NEEDS_STRING
   #ifndef POK_CONFIG_NEEDS_FUNC_MEMCPY
   #define POK_CONFIG_NEEDS_FUNC_MEMCPY         1
   #endif

   #ifndef POK_CONFIG_NEEDS_FUNC_MEMSET
   #define POK_CONFIG_NEEDS_FUNC_MEMSET         1
   #endif

   #ifndef POK_CONFIG_NEEDS_FUNC_MEMCMP
   #define POK_CONFIG_NEEDS_FUNC_MEMCMP         1
   #endif

   #ifndef POK_CONFIG_NEEDS_FUNC_STRCMP
   #define POK_CONFIG_NEEDS_FUNC_STRCMP         1
   #endif

   #ifndef POK_CONFIG_NEEDS_FUNC_STRNCMP
   #define POK_CONFIG_NEEDS_FUNC_STRNCMP        1
   #endif

   #ifndef POK_CONFIG_NEEDS_FUNC_STRCPY
   #define POK_CONFIG_NEEDS_FUNC_STRCPY         1
   #endif

   #ifndef POK_CONFIG_NEEDS_FUNC_STRNCPY
   #define POK_CONFIG_NEEDS_FUNC_STRNCPY        1
   #endif

   #ifndef POK_CONFIG_NEEDS_FUNC_STRLEN
   #define POK_CONFIG_NEEDS_FUNC_STRLEN         1
   #endif

   #ifndef POK_CONFIG_NEEDS_FUNC_STREQ
   #define POK_CONFIG_NEEDS_FUNC_STREQ          1
   #endif
#endif /* POK_NEEDS_STRING */

#ifdef POK_NEEDS_PROTOCOLS_DES
   #ifndef POK_CONFIG_NEEDS_FUNC_MEMCMP
   #define POK_CONFIG_NEEDS_FUNC_MEMCMP 1
   #endif

   #ifndef POK_CONFIG_NEEDS_FUNC_MEMCPY
   #define POK_CONFIG_NEEDS_FUNC_MEMCPY 1
   #endif
#endif


#ifdef POK_NEEDS_PROTOCOLS_BLOWFISH
   #ifndef POK_CONFIG_NEEDS_FUNC_MEMCPY
   #define POK_CONFIG_NEEDS_FUNC_MEMCPY 1
   #endif
#endif


#ifdef POK_NEEDS_ARINC653_PROCESS
   #define POK_NEEDS_THREADS 1
   #ifndef POK_CONFIG_NEEDS_FUNC_STRCPY
   #define POK_CONFIG_NEEDS_FUNC_STRCPY 1
   #endif

   #ifndef POK_CONFIG_NEEDS_FUNC_STRCMP
   #define POK_CONFIG_NEEDS_FUNC_STRCMP 1
   #endif

   #ifndef POK_NEEDS_THREAD_ID
   #define POK_NEEDS_THREAD_ID 1
   #endif
#endif

#ifdef POK_NEEDS_ARINC653_PARTITION
   #define POK_NEEDS_PARTITIONS 1
#endif

#ifdef POK_NEEDS_ARINC653_QUEUEING
   #define POK_NEEDS_PORTS_QUEUEING 1
#endif

#ifdef POK_NEEDS_ARINC653_SAMPLING
   #define POK_NEEDS_PORTS_SAMPLING 1
#endif


#ifdef POK_NEEDS_ARINC653_EVENT
   #ifndef POK_CONFIG_NEEDS_FUNC_STRNCMP
   #define POK_CONFIG_NEEDS_FUNC_STRNCMP 1
   #endif

   #ifndef POK_CONFIG_NEEDS_FUNC_STRLEN
   #define POK_CONFIG_NEEDS_FUNC_STRLEN 1
   #endif
#endif


#ifdef POK_NEEDS_ARINC653_BUFFER
   #ifndef POK_NEEDS_MIDDLEWARE
   #define POK_NEEDS_MIDDLEWARE 1
   #endif

   #ifndef POK_NEEDS_BUFFERS
   #define POK_NEEDS_BUFFERS 1
   #endif

   #ifndef POK_CONFIG_NEEDS_FUNC_STREQ
   #define POK_CONFIG_NEEDS_FUNC_STREQ 1
   #endif

   #ifndef POK_CONFIG_NEEDS_FUNC_STRLEN
   #define POK_CONFIG_NEEDS_FUNC_STRLEN 1
   #endif

   #ifndef POK_CONFIG_NEEDS_FUNC_STRLEN
   #define POK_CONFIG_NEEDS_FUNC_STRLEN 1
   #endif

#endif

#ifdef POK_NEEDS_ARINC653_BLACKBOARD
   #ifndef POK_NEEDS_MIDDLEWARE
   #define POK_NEEDS_MIDDLEWARE 1
   #endif

   #ifndef POK_NEEDS_BLACKBOARDS
   #define POK_NEEDS_BLACKBOARDS 1
   #endif

   #ifndef POK_CONFIG_NEEDS_FUNC_STREQ
   #define POK_CONFIG_NEEDS_FUNC_STREQ 1
   #endif

   #ifndef POK_CONFIG_NEEDS_FUNC_STRLEN
   #define POK_CONFIG_NEEDS_FUNC_STRLEN 1
   #endif

   #ifndef POK_CONFIG_NEEDS_FUNC_STRLEN
   #define POK_CONFIG_NEEDS_FUNC_STRLEN 1
   #endif
#endif

#ifdef POK_NEEDS_ARINC653_SEMAPHORE
   #define POK_NEEDS_SEMAPHORES                 1

   #ifndef POK_CONFIG_NEEDS_FUNC_STRNCMP
   #define POK_CONFIG_NEEDS_FUNC_STRNCMP 1
   #endif

   #ifndef POK_CONFIG_NEEDS_FUNC_STRLEN
   #define POK_CONFIG_NEEDS_FUNC_STRLEN 1
   #endif
#endif

#ifdef POK_NEEDS_ARINC653_ERROR
   #ifndef POK_NEEDS_ERROR_HANDLING
   #define POK_NEEDS_ERROR_HANDLING 1
   #endif

   #ifndef POK_NEEDS_FUNC_MEMCPY
   #define POK_CONFIG_NEEDS_FUNC_MEMCPY 1
   #endif
#endif

#if defined (POK_NEEDS_ERROR_HANDLING)
   #ifndef POK_NEEDS_FUNCTION_TIME_GETTICK
   #define POK_NEEDS_FUNCTION_TIME_GETTICK
   #endif
#endif

#ifdef POK_NEEDS_MUTEXES
   #ifndef POK_NEEDS_FUNCTION_TIME_GETTICK
   #define POK_NEEDS_FUNCTION_TIME_GETTICK
   #endif
#endif

#ifdef POK_NEEDS_EVENTS
   #ifndef POK_NEEDS_FUNCTION_TIME_GETTICK
   #define POK_NEEDS_FUNCTION_TIME_GETTICK
   #endif
#endif


#ifdef POK_NEEDS_BUFFERS
   #ifndef POK_NEEDS_FUNCTION_TIME_GETTICK
   #define POK_NEEDS_FUNCTION_TIME_GETTICK
   #endif

   #ifndef POK_NEEDS_FUNC_MEMCPY
   #define POK_CONFIG_NEEDS_FUNC_MEMCPY 1
   #endif

   #ifndef POK_CONFIG_NEEDS_FUNC_STREQ
   #define POK_CONFIG_NEEDS_FUNC_STREQ 1
   #endif

   #ifndef POK_CONFIG_NEEDS_FUNC_STRLEN
   #define POK_CONFIG_NEEDS_FUNC_STRLEN 1
   #endif
#endif

#ifdef POK_NEEDS_BLACKBOARDS
   #ifndef POK_NEEDS_FUNCTION_TIME_GETTICK
   #define POK_NEEDS_FUNCTION_TIME_GETTICK 1
   #endif

   #ifndef POK_CONFIG_NEEDS_FUNC_STREQ
   #define POK_CONFIG_NEEDS_FUNC_STREQ 1
   #endif

   #ifndef POK_CONFIG_NEEDS_FUNC_STRLEN
   #define POK_CONFIG_NEEDS_FUNC_STRLEN 1
   #endif

   #ifndef POK_NEEDS_FUNC_MEMCPY
   #define POK_CONFIG_NEEDS_FUNC_MEMCPY 1
   #endif
#endif

#ifdef POK_NEEDS_TIME
   #ifndef POK_NEEDS_FUNCTION_TIME_GETTICK
   #define POK_NEEDS_FUNCTION_TIME_GETTICK 1
   #endif
#endif
#endif /* POK_CONFIG_OPTIMIZE_FOR_GENERATED_CODE */

/*************************************************/

#ifndef POK_CONFIG_OPTIMIZE_FOR_GENERATED_CODE

/* core functions */
#ifndef POK_NEEDS_FUNCTIONS_TIME_GETTICK
#define POK_NEEDS_FUNCTION_TIME_GETTICK 1
#endif

#ifndef POK_NEEDS_SEMAPHORS
#define POK_NEEDS_SEMAPHORES            1
#endif

#ifndef POK_NEEDS_THREADS
#define POK_NEEDS_THREADS               1
#endif

#define POK_NEEDS_PORTS_QUEUEING        1
#define POK_NEEDS_PARTITIONS            1
#define POK_NEEDS_BUFFERS               1
#define POK_NEEDS_BLACKBOARDS           1
#define POK_NEEDS_SEMAPHORES            1

/* string.h functions */
#define POK_CONFIG_NEEDS_FUNC_MEMCPY         1
#define POK_CONFIG_NEEDS_FUNC_MEMSET         1
#define POK_CONFIG_NEEDS_FUNC_MEMCMP         1
#define POK_CONFIG_NEEDS_FUNC_STRCMP         1
#define POK_CONFIG_NEEDS_FUNC_STRNCMP        1
#define POK_CONFIG_NEEDS_FUNC_STRCPY         1
#define POK_CONFIG_NEEDS_FUNC_STRNCPY        1
#define POK_CONFIG_NEEDS_FUNC_STRLEN         1
#define POK_CONFIG_NEEDS_FUNC_STREQ          1
#define POK_CONFIG_NEEDS_FUNC_ITOA           1
#define POK_CONFIG_NEEDS_FUNC_UDIVDI3        1

/*
 * Memory allocator, core partition functions.
 */
#define POK_NEEDS_ALLOCATOR                  1

/* stdlib.h functions */
#define POK_CONFIG_NEEDS_FUNC_RAND           1
#define POK_CONFIG_NEEDS_FUNC_MALLOC         1
#define POK_CONFIG_NEEDS_FUNC_CALLOC         1
#define POK_CONFIG_NEEDS_FUNC_FREE           1

#endif /* POK_CONFIG_OPTIMIZE_FOR_GENERATED_CODE */

#endif
