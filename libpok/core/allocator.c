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
 * Created by julien on Thu Jul 30 15:49:30 2009 
 */

#include <core/dependencies.h>
#include <core/allocator.h>

/*
 * This is the allocator of POK. It remains in partition. You can configure it
 * with POK_CONFIG_ALLOCATOR_MEMORY_SIZE (total amount of memory that can be allocated
 * and with POK_CONFIG_ALLOCATOR_NB_SPACES (total amount of memory spaces that can be
 * allocated (number of successive calls to malloc() or calloc() for example.
 */

#ifdef POK_NEEDS_ALLOCATOR

#ifndef POK_CONFIG_ALLOCATOR_MEMORY_SIZE
#define POK_CONFIG_ALLOCATOR_MEMORY_SIZE 16384
#endif

#ifndef POK_CONFIG_ALLOCATOR_NB_SPACES
#define POK_CONFIG_ALLOCATOR_NB_SPACES 100
#endif

#define POK_ALLOCATOR_CHECK_INIT  \
                  if (pok_allocator_initialized == 0)\
                  { \
                     uint32_t toto; \
                     for (toto = 0 ; toto < POK_CONFIG_ALLOCATOR_NB_SPACES ; toto++) \
                     { \
                        pok_allocator_spaces[toto].start = 0; \
                        pok_allocator_spaces[toto].size  = 0; \
                        pok_allocator_spaces[toto].allocated  = 0; \
                     } \
                     pok_allocator_used_spaces = 1; \
                     pok_allocator_spaces[0].start = 0; /* Initialize the first space */ \
                     pok_allocator_spaces[0].allocated = 0; \
                     pok_allocator_spaces[0].size = POK_CONFIG_ALLOCATOR_MEMORY_SIZE;\
                     pok_allocator_initialized = 1; \
                  }

/* POK_ALLOCATOR_CHECK_INIT is a macro that performs initialization functions
 * for memory allocator. It is called each time alloc or free functions are called
 * but initialized the memory allocator only one time.
 */


typedef struct
{
   size_t   start;
   size_t   size;
   bool_t   allocated;
} pok_allocator_space_t;


uint8_t                 pok_allocator_memspace[POK_CONFIG_ALLOCATOR_MEMORY_SIZE];
pok_allocator_space_t   pok_allocator_spaces[POK_CONFIG_ALLOCATOR_NB_SPACES];
uint32_t                pok_allocator_used_spaces = 0;
bool_t                  pok_allocator_initialized = 0;

#ifdef POK_NEEDS_DEBUG
void pok_allocator_print_spaces ()
{
   uint32_t space;
   printf ("[LIBPOK] [ALLOCATOR] Used spaces = %d\n", pok_allocator_used_spaces);
   for (space = 0 ; space < pok_allocator_used_spaces ; space++)
   {
      printf ("[LIBPOK] [ALLOCATOR] Space %d start=%d size=%d allocated=%d\n", space, pok_allocator_spaces[space].start, pok_allocator_spaces[space].size, pok_allocator_spaces[space].allocated);
   }
}
#endif

void* pok_allocator_allocate (size_t needed_size)
{
   POK_ALLOCATOR_CHECK_INIT
   uint32_t space;
   uint32_t new_space;

   if (pok_allocator_used_spaces >= (POK_CONFIG_ALLOCATOR_NB_SPACES - 1))
   {
#ifdef POK_NEEDS_DEBUG
      printf ("[LIBPOK] [ALLOCATOR] Not enough space\n");
#endif
      return NULL;
   }

#ifdef POK_NEEDS_DEBUG
   printf("Try to take a new memory chunk, required space=%d\n", needed_size);
#endif

   for (space = 0 ; space < pok_allocator_used_spaces ; space++)
   {
#ifdef POK_NEEDS_DEBUG
      printf ("[LIBPOK] [ALLOCATOR] Look space %d, size %d, allocated=%d\n", space, pok_allocator_spaces[space].size, pok_allocator_spaces[space].allocated);
#endif
      if ((pok_allocator_spaces[space].allocated == 0) && (pok_allocator_spaces[space].size >= needed_size))
      {
         if (pok_allocator_spaces[space].size == needed_size)
         {
            /*
             * The space corresponds exactly to the requested memory space
             */
            pok_allocator_spaces[space].allocated = 1;

#ifdef POK_NEEDS_DEBUG
            printf ("[LIBPOK] [ALLOCATOR] Allocate directly space %d\n", space);
            pok_allocator_print_spaces ();
#endif
            return (&pok_allocator_memspace[pok_allocator_spaces[space].start]);
         }
         else
         {
            /*
             * We need to split the block in two new blocks !
             */
            new_space = pok_allocator_used_spaces;
            pok_allocator_used_spaces = pok_allocator_used_spaces + 1;

            pok_allocator_spaces[space].allocated        = 1;
            pok_allocator_spaces[new_space].allocated    = 0;

            pok_allocator_spaces[new_space].size         = pok_allocator_spaces[space].size - needed_size;
            pok_allocator_spaces[space].size             = needed_size;

            pok_allocator_spaces[new_space].start        = pok_allocator_spaces[space].start + needed_size;

#ifdef POK_NEEDS_DEBUG
            printf("[LIBPOK] [ALLOCATOR] Allocate space %d, CREATE NEW SPACE %d (size=%d)\n", space, new_space, pok_allocator_spaces[new_space].size);
            pok_allocator_print_spaces ();
#endif

            return (&pok_allocator_memspace[pok_allocator_spaces[space].start]);
         }
      }
   }

#ifdef POK_NEEDS_DEBUG
      printf ("[LIBPOK] [ALLOCATOR] Didn't find any space for that amount of memory (%d)\n", needed_size);
#endif
   return NULL;
}

/*
 * Delete a space
 */
void pok_allocator_delete_space (uint32_t space)
{
   uint32_t tmp;

#ifdef POK_NEEDS_DEBUG
            printf("[LIBPOK] [ALLOCATOR] Delete space %d\n", space);
#endif

   for (tmp = space ; tmp < POK_CONFIG_ALLOCATOR_NB_SPACES ; tmp++)
   {
      pok_allocator_spaces[tmp].allocated = pok_allocator_spaces[tmp+1].allocated;
      pok_allocator_spaces[tmp].size      = pok_allocator_spaces[tmp+1].size;
      pok_allocator_spaces[tmp].start     = pok_allocator_spaces[tmp+1].start;
   }

   pok_allocator_used_spaces = pok_allocator_used_spaces - 1;
}

void pok_allocator_merge_space (uint32_t space)
{
   uint32_t space2;

   if (pok_allocator_used_spaces == 1)
   {
      return;
   }

   for (space2 = 0 ; space2 < pok_allocator_used_spaces ; space2++)
   {
      if ((space2 == space) || (pok_allocator_spaces[space2].allocated == 1))
      {
         continue;
      }

      /*
       * In that case, space is a the end of space2. We can merge space in space2
       */
      if (pok_allocator_spaces[space].start == (pok_allocator_spaces[space2].start + pok_allocator_spaces[space2].size))
      {
#ifdef POK_NEEDS_DEBUG
//         printf("[LIBPOK] [ALLOCATOR] Merge space %d, with %d\n", space, space2);
#endif
         pok_allocator_spaces[space2].size = pok_allocator_spaces[space2].size + pok_allocator_spaces[space].size;
         pok_allocator_delete_space (space);
         pok_allocator_merge_space (space2);
         return;
      }

      /*
       * In that case, space2 is located at the end of space. We can merge space2 in space
       */
      if (pok_allocator_spaces[space2].start == (pok_allocator_spaces[space].start + pok_allocator_spaces[space].size))
      {
#ifdef POK_NEEDS_DEBUG
//         printf("[LIBPOK] [ALLOCATOR] Merge space %d, with %d\n", space, space2);
#endif
         pok_allocator_spaces[space].size = pok_allocator_spaces[space].size + pok_allocator_spaces[space2].size;
         pok_allocator_delete_space (space2);
         pok_allocator_merge_space (space2);
         return;
      }
   }
}


void pok_allocator_free (void* ptr)
{
   POK_ALLOCATOR_CHECK_INIT

   uint32_t space;

   for (space = 0 ; space < pok_allocator_used_spaces ; space++)
   {
      if (ptr == &pok_allocator_memspace[pok_allocator_spaces[space].start])
      {
         /* Here, we find the space to free */
         pok_allocator_spaces[space].allocated = 0;
         pok_allocator_merge_space (space);
#ifdef POK_NEEDS_DEBUG
         pok_allocator_print_spaces ();
#endif
         return;
      }
   }

#ifdef POK_NEEDS_DEBUG
   printf("[LIBPOK] [ALLOCATOR] free() didn't find the space to free\n");
   pok_allocator_print_spaces ();
#endif
   return;
}

#endif

