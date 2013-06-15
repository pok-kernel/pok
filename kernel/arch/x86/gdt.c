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


#include <libc.h>

#include <types.h>
#include <errno.h>

#include "gdt.h"
#include "sysdesc.h"
#include "tss.h"

#ifndef POK_NEEDS_THREADS
#define POK_CONFIG_NB_THREADS 0
#endif

#ifndef POK_NEEDS_PARTITIONS
#define POK_CONFIG_NB_PARTITIONS 0
#endif

#define GDT_SIZE 256

gdt_entry_t	pok_gdt[GDT_SIZE];

tss_t		pok_tss;

pok_ret_t pok_gdt_init()
{
   sysdesc_t sysdesc;

   /* Set null descriptor and clear table */
   memset(pok_gdt, 0, sizeof (gdt_entry_t) * GDT_SIZE);

   /* Set kernel descriptors */
   gdt_set_segment(GDT_CORE_CODE_SEGMENT, 0, ~0UL, GDTE_CODE, 0);
   gdt_set_segment(GDT_CORE_DATA_SEGMENT, 0, ~0UL, GDTE_DATA, 0);

   /* Load GDT */
   sysdesc.limit = sizeof (pok_gdt);
   sysdesc.base = (uint32_t)pok_gdt;

   asm ("lgdt %0"
         :
         : "m" (sysdesc));

   /* Reload Segments */
   asm ("ljmp %0, $1f	\n"
         "1:		\n"
         "mov %1, %%ax	\n"
         "mov %%ax, %%ds	\n"
         "mov %%ax, %%es	\n"
         "mov %%ax, %%fs	\n"
         "mov %%ax, %%gs	\n"
         "mov %%ax, %%ss	\n"
         :
         : "i" (GDT_CORE_CODE_SEGMENT << 3),
         "i" (GDT_CORE_DATA_SEGMENT << 3)
         : "eax");

   pok_tss_init();

   return (POK_ERRNO_OK);
}

int pok_tss_init()
{
   uint16_t sel = GDT_BUILD_SELECTOR(GDT_TSS_SEGMENT, 0, 0);

   memset(&pok_tss, 0, sizeof (tss_t));

   pok_tss.ss0 = GDT_BUILD_SELECTOR(GDT_CORE_DATA_SEGMENT, 0, 0);

   gdt_set_system(GDT_TSS_SEGMENT, (uint32_t)&pok_tss,
         sizeof (tss_t), GDTE_TSS, 0);

   asm ("ltr %0" : :"m"(sel));
   return (POK_ERRNO_OK);
}

void tss_set_esp0(uint32_t esp0)
{
   pok_tss.esp0 = esp0;
}

void gdt_set_segment(uint16_t index,
      uint32_t base_address,
      uint32_t limit,
      e_gdte_type t,
      int dpl)
{
   if (limit > (1 << 20)) /* 4K granularity */
   {
      pok_gdt[index].limit_low = (limit >> 12) & 0xFFFF;
      pok_gdt[index].limit_high = (limit >> 28) & 0xF;
      pok_gdt[index].granularity = 1;
   }
   else /* 1B granularity */
   {
      pok_gdt[index].limit_low = limit & 0xFFFF;
      pok_gdt[index].limit_high = (limit >> 16) & 0xFF;
      pok_gdt[index].granularity = 0;
   }

   pok_gdt[index].base_low = base_address & 0xFFFFFF;
   pok_gdt[index].base_high = (base_address >> 24) & 0xFF;

   pok_gdt[index].type = t & 0xF;
   pok_gdt[index].dpl = dpl & 0x3;

   pok_gdt[index].s = 1;		      /* Segment is data/code type */
   pok_gdt[index].present = 1;
   pok_gdt[index].available = 0;
   pok_gdt[index].op_size = 1;	      /* We work on 32 bits segments */
}

void gdt_set_system(uint16_t index,
      uint32_t base_address,
      uint32_t limit,
      e_gdte_type t,
      int dpl)
{
   pok_gdt[index].limit_low = limit & 0xFFFF;
   pok_gdt[index].limit_high = (limit >> 16) & 0xFF;
   pok_gdt[index].base_low = base_address & 0xFFFFFF;
   pok_gdt[index].base_high = (base_address >> 24) & 0xFF;

   pok_gdt[index].type = t & 0xF;
   pok_gdt[index].dpl = dpl & 0x3;

   pok_gdt[index].s = 0;		      /* Segment is system type */
   pok_gdt[index].present = 1;
   pok_gdt[index].available = 0;
   pok_gdt[index].op_size = 0;
}

void gdt_enable(uint16_t index)
{
   pok_gdt[index].present = 1;
}

void gdt_disable(uint16_t index)
{
   pok_gdt[index].present = 0;
}

/*
 * DEPRECATED
 *
uint32_t	gdt_segment_base(uint16_t idx)
{
   uint32_t base;

   base = pok_gdt[idx].base_low | (pok_gdt[idx].base_high << 24);

   return base;
}
*/

